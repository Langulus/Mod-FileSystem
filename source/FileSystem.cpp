///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "FileSystem.hpp"

LANGULUS_DEFINE_MODULE(
   FileSystem, 9, "FileSystem",
   "File system", "",
   FileSystem, File, Folder
)


/// Module construction                                                       
///   @param runtime the runtime that owns the module                         
///   @param descriptor instructions for configuring the module               
FileSystem::FileSystem(Runtime* runtime, Many const&)
   : Downcastable {this}
   , Module       {runtime} {
   VERBOSE_VFS("Initializing...");

   // Initialize the virtual file system                                
   LglsAssert(0 != PHYSFS_init(nullptr), "Error initializing file system"
      " due to PHYSFS_init error: ", GetLastError());
   VERBOSE_VFS("PhysFS initialized");

   // Get the working and data directories                              
   // This is the only place where full paths are used                  
   //TODO erase working path after mounting, if LANGULUS(PARANOID)
   mWorkingPath = PHYSFS_getBaseDir();
   mMainDataPath = "data";
   VERBOSE_VFS("Detected working path: `", mWorkingPath, '`');

   // Mount main read/write path                                        
   const auto dataio = mWorkingPath / mMainDataPath;
   LglsAssert(0 != PHYSFS_mount(dataio.GetRaw(), nullptr, 0),
      "Can't mount main data directory `", dataio,
      "` due to PHYSFS_mount error: ", GetLastError()
   );
   VERBOSE_VFS("Mounted main data directory: ", dataio);

   // Set main write path                                               
   if (0 == PHYSFS_setWriteDir(dataio.GetRaw())) {
      Logger::Warning(this, ": Can't set write directory: ", dataio);
      Logger::Warning(this,
         "File writing will be disabled, due to PHYSFS_setWriteDir error: ",
         GetLastError()
      );
   }
   else {
      VERBOSE_VFS("Mounted main write directory: ", dataio);
   }

   // Log supported file types                                          
   auto _ = Logger::InfoScoped(this, ": Supports:");
   auto supported = PHYSFS_supportedArchiveTypes();
   while (*supported) {
      Logger::Line(
         (*supported)->extension, " files -- ", 
         (*supported)->description, " (",
         (*supported)->author, "; ",
         (*supported)->url, ")"
      );
      ++supported;
   }
   VERBOSE_VFS("Initialized");
}

/// Shutdown PhysFS down                                                      
FileSystem::~FileSystem() {
   VERBOSE_VFS("Destroying...");
   LglsAssert(0 != PHYSFS_deinit(),
      "Error deinitializing file system due to PHYSFS_deinit error: ",
      GetLastError()
   );
   VERBOSE_VFS("PhysFS deinitialized");
   VERBOSE_VFS("Destroyed");
}

/// First-stage destruction that severs all interlinked memory                
void FileSystem::Teardown() {
   mFolderMap.Reset();
   mFileMap.Reset();
   mWorkingPath.Reset();
   mMainDataPath.Reset();
   mFiles.Teardown();
   mFolders.Teardown();

   // Release all files before shutting physfs down, otherwise handles  
   // stored in files will become unclosable                            
   mFolders.Reset();
   mFiles.Reset();
}

/// Module update routine                                                     
///   @param dt time from last update                                         
bool FileSystem::Update(Time) {
   return true;
}

/// Create/Destroy file and folder interfaces                                 
///   @param verb the creation/destruction verb                               
void FileSystem::Create(Verb& verb) {
   mFiles.Create(this, verb);
   mFolders.Create(this, verb);
}

/// Select file and folder interfaces                                         
///   @param verb the selection verb                                          
void FileSystem::Select(Verb& verb) {
   mFiles.Select(verb);
   mFolders.Select(verb);
}

/// Interface a file                                                          
/// This doesn't open the file, nor ensures the file exists - it only creates 
/// an object, that can do those things                                       
///   @param path the filename to interface                                   
///   @return the file interface or nullptr on failure                        
auto FileSystem::GetFile(Path const& path) const -> Ref<Things::File> {
   if (not path)
      return {};

   // Check if file is already interfaced                               
   const auto normalizedPath = path.Lowercase();
   auto found = mFileMap.Find(normalizedPath);
   if (found)
      return found.GetVal();

   // Produce a new file interface                                      
   Verbs::Create creator {Recipe::For<File>(normalizedPath)};
   mFiles.Create(this, creator);
   if (creator.IsDone()) {
      auto filePtr = creator->template As<Things::File*>();
      mFileMap.Merge(normalizedPath, filePtr);
      return filePtr;
   }

   return {};
}

/// Interface a folder                                                        
/// This doesn't open the folder, nor ensures the file exists - it only       
/// creates an object, that can do those things                               
///   @param path the directory path to interface                             
///   @return the file interface or nullptr on failure                        
auto FileSystem::GetFolder(Path const& path) const -> Ref<Things::Folder> {
   if (not path)
      return {};

   // Check if folder is already interfaced                             
   const auto normalizedPath = path.Lowercase();
   auto found = mFolderMap.Find(normalizedPath);
   if (found)
      return found.GetVal();

   // Produce a new folder interface                                    
   Verbs::Create creator {Recipe::For<Folder>(normalizedPath)};
   mFolders.Create(this, creator);
   if (creator.IsDone()) {
      auto folderPtr = creator->template As<Things::Folder*>();
      mFolderMap.Merge(normalizedPath, folderPtr);
      return folderPtr;
   }

   return {};
}
