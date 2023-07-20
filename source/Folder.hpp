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


///                                                                           
///   A folder                                                                
///                                                                           
struct Folder final : A::Folder, ProducedFrom<FileSystem> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) FileSystem;
   LANGULUS_BASES(A::Folder);
   LANGULUS_VERBS(Verbs::Create);

private:
   Path mFolderPath;

public:
   Folder(FileSystem*, const Descriptor&);
   ~Folder();

   void Refresh();
   void Create(Verb&);

   NOD() const A::File* GetFile(const Path&) const;
   NOD() const A::Folder* GetFolder(const Path&) const;
};