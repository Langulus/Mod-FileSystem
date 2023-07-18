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
   Logger::Verbose(Self(), "Initializing...");
   Logger::Verbose(Self(), "Initialized");
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

const A::File* FileSystem::GetFile(const Path&) const {

}

const A::Folder* FileSystem::GetFolder(const Path&) const {

}
