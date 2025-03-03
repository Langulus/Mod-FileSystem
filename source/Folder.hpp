///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Common.hpp"
#include <Langulus/Flow/Producible.hpp>
#include <Langulus/Verbs/Create.hpp>
#include <Langulus/Verbs/Select.hpp>


///                                                                           
///   A folder                                                                
///                                                                           
struct Folder final : A::Folder, ProducedFrom<FileSystem> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) FileSystem;
   LANGULUS_BASES(A::Folder);
   LANGULUS_VERBS(Verbs::Create, Verbs::Select);

private:
   // Information about the folder, if it exists                        
   PHYSFS_Stat mFolderInfo {};

public:
   Folder(FileSystem*, const Many&);

   void Create(Verb&);
   void Select(Verb&);

   void Refresh();
   void Teardown();

   auto RelativeFile  (const Path&) const -> Ref<A::File>;
   auto RelativeFolder(const Path&) const -> Ref<A::Folder>;
};