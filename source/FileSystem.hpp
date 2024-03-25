///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "File.hpp"
#include "Folder.hpp"
#include <Flow/Verbs/Create.hpp>


///                                                                           
///   File system                                                             
///                                                                           
struct FileSystem final : A::FileSystem {
   LANGULUS(ABSTRACT) false;
   LANGULUS_BASES(A::FileSystem);
   LANGULUS_VERBS(Verbs::Create, Verbs::Select);

private:
   // List of interfaced files                                          
   TFactoryUnique<File> mFiles;
   // Files indexed by a lowercase relative path                        
   TUnorderedMap<Path, Ref<A::File>> mFileMap;

   // List of interfaced folders                                        
   TFactoryUnique<Folder> mFolders;
   // Folders indexed by a lowercase relative path                      
   TUnorderedMap<Path, Ref<A::Folder>> mFolderMap;

public:
   FileSystem(Runtime*, const Neat&);
   ~FileSystem();

   bool Update(Time);
   void Detach();

   void Create(Verb&);
   void Select(Verb&);

   NOD() Ref<A::File>    GetFile(const Path&);
   NOD() Ref<A::Folder>  GetFolder(const Path&);
};

