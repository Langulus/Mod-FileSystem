///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright(C) 2023 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Folder.hpp"
#include "FileSystem.hpp"


/// Folder constructor                                                        
///   @param producer - the folder producer                                   
///   @param descriptor - instructions for configuring the folder             
Folder::Folder(FileSystem* producer, const Neat& descriptor)
   : A::Folder {MetaOf<::Folder>(), descriptor}
   , ProducedFrom {producer, descriptor} {
   VERBOSE_VFS("Initializing...");

   // Get a path from the descriptor                                    
   if (not descriptor.ExtractTrait<Traits::Name, Traits::Path>(mFolderPath))
      descriptor.ExtractDataAs(mFolderPath);
   LANGULUS_ASSERT(mFolderPath, FileSystem,
      "Can't interface empty directory path");

   // Check if folder exists, and retrieve its info                     
   if (0 != PHYSFS_stat(mFolderPath.Terminate().GetRaw(), &mFolderInfo)) {
      LANGULUS_ASSERT(
         mFolderInfo.filetype == PHYSFS_FILETYPE_DIRECTORY, FileSystem,
         "Path `", mFolderPath, "` doesn't point to a regular directory"
      );

      mExists = true;
      mIsReadOnly = mFolderInfo.readonly;
      VERBOSE_VFS("Interfaces existing directory: ", mFolderPath);
   }
   else {
      VERBOSE_VFS("Interfaces non-existing directory: ", mFolderPath);
   }

   VERBOSE_VFS("Initialized");
}

/// React on environmental change                                             
void Folder::Refresh() {}

/// Create or delete files and subfolders under this folder                   
void Folder::Create(Verb&) {
   TODO();
}

/// Select files and subfolders under this folder                             
void Folder::Select(Verb&) {
   TODO();
}

/// Get a file interface, from a path that resides in this folder             
///   @param filename - path relative to this folder                          
///   @return the file interface                                              
Ref<A::File> Folder::GetFile(const Path& filename) const {
   return GetProducer()->GetFile(mFolderPath / filename);
}

/// Get a subfolder interface, from a path that resides in this folder        
///   @param dirname - path relative to this folder                           
///   @return the file interface                                              
Ref<A::Folder> Folder::GetFolder(const Path& dirname) const {
   return GetProducer()->GetFolder(mFolderPath / dirname);
}
