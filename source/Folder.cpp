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
///   @param descriptor - instructions for configuring the GUI                
Folder::Folder(FileSystem* producer, const Descriptor& descriptor)
   : A::Folder {MetaOf<::Folder>(), descriptor}
   , ProducedFrom {producer, descriptor} {
   VERBOSE_VFS("Initializing...");
   VERBOSE_VFS("Initialized");
}

/// File destruction                                                          
Folder::~Folder() {

}

/// React on environmental change                                             
void Folder::Refresh() {
   TODO();
}

void Folder::Create(Verb&) {
   TODO();
}

const A::File* Folder::GetFile(const Path&) const {
   TODO();
}

const A::Folder* Folder::GetFolder(const Path&) const {
   TODO();
}
