///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright(C) 2023 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"
#include <Flow/Verbs/Create.hpp>
#include <Flow/Verbs/Select.hpp>


///                                                                           
///   A folder                                                                
///                                                                           
struct Folder final : A::Folder, ProducedFrom<FileSystem> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) FileSystem;
   LANGULUS_BASES(A::Folder);
   LANGULUS_VERBS(Verbs::Create, Verbs::Select);

private:
   Path mFolderPath;

   // Information about the folder, if it exists                        
   PHYSFS_Stat mFolderInfo {};

public:
   Folder(FileSystem*, const Neat&);

   void Refresh();
   void Create(Verb&);
   void Select(Verb&);

   NOD() Ref<A::File> GetFile(const Path&) const;
   NOD() Ref<A::Folder> GetFolder(const Path&) const;
};