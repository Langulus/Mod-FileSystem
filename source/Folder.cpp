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

}

/// File destruction                                                          
Folder::~Folder() {

}

/// React on environmental change                                             
void Folder::Refresh() {

}

void Folder::Create(Verb&) {

}

const A::File* Folder::GetFile(const Path&) const {

}

const A::Folder* Folder::GetFolder(const Path&) const {

}
