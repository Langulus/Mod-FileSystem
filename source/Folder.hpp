///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Export.hpp"
#include <Langulus/Path.hpp>
#include <Langulus/Producible.hpp>
#include <Langulus/Verbs/Create.hpp>
#include <Langulus/Verbs/Select.hpp>


///                                                                           
///   A folder                                                                
///                                                                           
struct Folder final : Things::Folder, Flow::ProducedFrom<FileSystem> {
   using CTTI_Abstract  = No;
   using CTTI_Producer  = FileSystem;
   using CTTI_Bases     = Things::Folder;
   using CTTI_Ability   = Types<Verbs::Create, Verbs::Select>;

private:
   // Information about the folder, if it exists                        
   PHYSFS_Stat mFolderInfo {};

public:
   Folder(FileSystem*, Many const&);

   void Create(Verb&);
   void Select(Verb&);

   void Teardown();

   void Refresh() override;
   auto RelativeFile  (Path const&) const -> Ref<Things::File> override;
   auto RelativeFolder(Path const&) const -> Ref<Things::Folder> override;
};