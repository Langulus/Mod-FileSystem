///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright(C) 2023 Dimo Markov <langulusteam@gmail.com>                    
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
   TFactoryUnique<File>          mFiles;
   // Files indexed by a lowercase relative path                        
   TUnorderedMap<Path, File*>    mFileMap;

   // List of interfaced folders                                        
   TFactoryUnique<Folder>        mFolders;
   // Folders indexed by a lowercase relative path                      
   TUnorderedMap<Path, Folder*>  mFolderMap;

public:
   FileSystem(Runtime*, const Descriptor&);
   ~FileSystem();

   void Update(Time);

   void Create(Verb&);
   void Select(Verb&);

   NOD() const A::File*    GetFile(const Path&) const;
   NOD() const A::Folder*  GetFolder(const Path&) const;
};

