///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright(C) 2023 Dimo Markov <langulusteam@gmail.com>                    
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
FileSystem::FileSystem(Runtime* runtime, const Descriptor&)
   : A::FileSystem {MetaOf<::FileSystem>(), runtime}
   , mFiles {this}
   , mFolders {this} {
   VERBOSE_VFS("Initializing...");

   // Initialize the virtual file system                                
   if (0 == PHYSFS_init(nullptr)) {
      LANGULUS_ASSERT(
         false, FileSystem, "Error initializing file system",
         " due to PHYSFS_init error: ", GetLastError()
      );
   }

   // Get the working directory                                         
   mWorkingPath = PHYSFS_getBaseDir();
   VERBOSE_VFS("Detected working path: ", mWorkingPath);

   // Mount main read/write path                                        
   mMainDataPath = mWorkingPath / "Data";
   if (0 == PHYSFS_mount(mMainDataPath.GetRaw(), nullptr, 0)) {
      LANGULUS_ASSERT(
         false, FileSystem, "Can't mount main data path",
         " due to PHYSFS_mount error: ", GetLastError()
      );
   }
   VERBOSE_VFS("Mounted main data path: ", mMainDataPath);

   // Mount main write path                                             
   mMainCachePath = mMainDataPath / "Cache";
   if (0 == PHYSFS_mount(mMainCachePath.GetRaw(), nullptr, 0)) {
      LANGULUS_ASSERT(
         false, FileSystem, "Can't mount main cache path",
         " due to PHYSFS_mount error: ", GetLastError()
      );
   }
   VERBOSE_VFS("Mounted main cache directory: ", mMainCachePath);

   if (0 == PHYSFS_setWriteDir(mMainCachePath.GetRaw())) {
      Logger::Warning(Self(),
         "Can't set write directory: ", mMainCachePath);
      Logger::Warning(Self(),
         "File writing will be disabled, due to PHYSFS_setWriteDir error: ",
         GetLastError()
      );
   }
   else VERBOSE_VFS("Mounted main write directory: ", mMainCachePath);

   // Log supported file types                                          
   auto supported = PHYSFS_supportedArchiveTypes();
   while (*(supported++)) {
      Logger::Info(Self(), "Supports `", 
         (*supported)->extension, "` files -- ", 
         (*supported)->description, " (",
         (*supported)->author, "; ",
         (*supported)->url, ")"
      );
   }
   VERBOSE_VFS("Initialized");
}

/// Shutdown PhysFS                                                           
FileSystem::~FileSystem() {
   // Release all files before shutting physfs down                     
   mFileMap.Reset();
   mFiles.Reset();
   mFolderMap.Reset();
   mFolders.Reset();

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
void FileSystem::Update(Time) {

}

/// Create/Destroy file and folder interfaces                                 
///   @param verb - the creation/destruction verb                             
void FileSystem::Create(Verb& verb) {
   mFiles.Create(verb);
   mFolders.Create(verb);
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
const A::File* FileSystem::GetFile(const Path& path) const {
   if (path.IsEmpty())
      return nullptr;

   // Check if file is already interfaced                               
   const auto normalizedPath = path.Lowercase();
   auto found = mFileMap.Find(normalizedPath);
   if (found)
      return mFileMap.GetValue(found);

   // Produce a new file interface                                      
   Verbs::Create creator {Construct::From<File>(path)};
   const_cast<TFactoryUnique<File>&>(mFiles).Create(creator);
   if (creator.IsDone()) {
      auto filePtr = creator.GetOutput().As<File*>();
      const_cast<TUnorderedMap<Path, File*>&>(mFileMap)
         [normalizedPath] = filePtr;
      return filePtr;
   }

   return nullptr;
}

/// Interface a folder                                                        
/// This doesn't open the folder, nor ensures the file exists - it only       
/// creates an object, that can do those things                               
///   @param path - the directory path to interface                           
///   @return the file interface or nullptr on failure                        
const A::Folder* FileSystem::GetFolder(const Path& path) const {
   if (path.IsEmpty())
      return nullptr;

   // Check if folder is already interfaced                             
   const auto normalizedPath = path.Lowercase();
   auto found = mFolderMap.Find(normalizedPath);
   if (found)
      return mFolderMap.GetValue(found);

   // Produce a new folder interface                                    
   Verbs::Create creator {Construct::From<Folder>(path)};
   const_cast<TFactoryUnique<Folder>&>(mFolders).Create(creator);
   if (creator.IsDone()) {
      auto folderPtr = creator.GetOutput().As<Folder*>();
      const_cast<TUnorderedMap<Path, Folder*>&>(mFolderMap)
         [normalizedPath] = folderPtr;
      return folderPtr;
   }

   return nullptr;
}
