///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "FileSystem.hpp"

LANGULUS_DEFINE_MODULE(
   FileSystem, 9, "FileSystem",
   "File system", "",
   FileSystem, File, Folder
)


/// Module construction                                                       
///   @param runtime - the runtime that owns the module                       
///   @param descriptor - instructions for configuring the module             
FileSystem::FileSystem(Runtime* runtime, const Neat&)
   : Resolvable {this}
   , Module     {runtime} {
   VERBOSE_VFS("Initializing...");

   // Initialize the virtual file system                                
   if (0 == PHYSFS_init(nullptr)) {
      LANGULUS_OOPS(FileSystem, "Error initializing file system",
         " due to PHYSFS_init error: ", GetLastError());
   }

   // Get the working and data directories                              
   // This is the only place where full paths are used                  
   //TODO erase working path after mounting, if LANGULUS(PARANOID)
   mWorkingPath = PHYSFS_getBaseDir();
   mMainDataPath = "data";
   VERBOSE_VFS("Detected working path: `", mWorkingPath, '`');

   // Mount main read/write path                                        
   const auto dataio = (mWorkingPath / mMainDataPath).Terminate();
   if (0 == PHYSFS_mount(dataio.GetRaw(), nullptr, 0)) {
      Logger::Error(Self(),
         "Can't mount main data directory `", dataio,
         "` due to PHYSFS_mount error: ", GetLastError());
      Detach();
      LANGULUS_THROW(FileSystem, "Can't mount main data directory");
   }

   VERBOSE_VFS("Mounted main data directory: ", dataio);

   // Set main write path                                               
   if (0 == PHYSFS_setWriteDir(dataio.GetRaw())) {
      Logger::Warning(Self(), "Can't set write directory: ", dataio);
      Logger::Warning(Self(),
         "File writing will be disabled, due to PHYSFS_setWriteDir error: ",
         GetLastError()
      );
   }
   else {
      VERBOSE_VFS("Mounted main write directory: ", dataio);
   }

   // Log supported file types                                          
   auto supported = PHYSFS_supportedArchiveTypes();
   while (*supported) {
      Logger::Info(Self(), "Supports `", 
         (*supported)->extension, "` files -- ", 
         (*supported)->description, " (",
         (*supported)->author, "; ",
         (*supported)->url, ")"
      );
      ++supported;
   }
   VERBOSE_VFS("Initialized");
}

/// Shutdown PhysFS                                                           
FileSystem::~FileSystem() {
   Detach();
}

void FileSystem::Detach() {
   // Release all files before shutting physfs down                     
   mFolderMap.Reset();
   mFileMap.Reset();

   mFolders.Reset();
   mFiles.Reset();

   // Shut PhysFS down                                                  
   if (0 == PHYSFS_deinit()) {
      Logger::Error(Self(),
         "Error deinitializing file system due to PHYSFS_deinit error: ",
         GetLastError()
      );
   }
}

/// Module update routine                                                     
///   @param dt - time from last update                                       
bool FileSystem::Update(Time) {
   return true;
}

/// Create/Destroy file and folder interfaces                                 
///   @param verb - the creation/destruction verb                             
void FileSystem::Create(Verb& verb) {
   mFiles.Create(this, verb);
   mFolders.Create(this, verb);
}

/// Select file and folder interfaces                                         
///   @param verb - the selection verb                                        
void FileSystem::Select(Verb& verb) {
   mFiles.Select(verb);
   mFolders.Select(verb);
}

/// Interface a file                                                          
/// This doesn't open the file, nor ensures the file exists - it only creates 
/// an object, that can do those things                                       
///   @param path - the filename to interface                                 
///   @return the file interface or nullptr on failure                        
Ref<A::File> FileSystem::GetFile(const Path& path) {
   if (not path)
      return {};

   // Check if file is already interfaced                               
   const auto normalizedPath = path.Lowercase();
   auto found = mFileMap.FindIt(normalizedPath);
   if (found)
      return *found.mValue;

   // Produce a new file interface                                      
   Verbs::Create creator {Construct::From<File>(normalizedPath)};
   mFiles.Create(this, creator);
   if (creator.IsDone()) {
      auto filePtr = creator->template As<A::File*>();
      mFileMap.Insert(normalizedPath, filePtr);
      return filePtr;
   }

   return {};
}

/// Interface a folder                                                        
/// This doesn't open the folder, nor ensures the file exists - it only       
/// creates an object, that can do those things                               
///   @param path - the directory path to interface                           
///   @return the file interface or nullptr on failure                        
Ref<A::Folder> FileSystem::GetFolder(const Path& path) {
   if (not path)
      return {};

   // Check if folder is already interfaced                             
   const auto normalizedPath = path.Lowercase();
   auto found = mFolderMap.FindIt(normalizedPath);
   if (found)
      return *found.mValue;

   // Produce a new folder interface                                    
   Verbs::Create creator {Construct::From<Folder>(normalizedPath)};
   mFolders.Create(this, creator);
   if (creator.IsDone()) {
      auto folderPtr = creator->template As<A::Folder*>();
      mFolderMap.Insert(normalizedPath, folderPtr);
      return folderPtr;
   }

   return {};
}
