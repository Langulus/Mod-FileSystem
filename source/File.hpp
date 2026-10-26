///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Export.hpp"
#include <Langulus/TOwn.hpp>
#include <Langulus/Path.hpp>
#include <Langulus/Producible.hpp>
#include <Langulus/Verbs/Associate.hpp>
#include <Langulus/Verbs/Catenate.hpp>
#include <Langulus/Verbs/Select.hpp>
#include <Langulus/Verbs/Interpret.hpp>
//#include <optional>


///                                                                           
///   A file                                                                  
struct File final : Things::File, ProducedFrom<FileSystem> {
   using CTTI_Abstract  = No;
   using CTTI_Producer  = FileSystem;
   using CTTI_Bases     = Things::File;
   using CTTI_Ability   = Types<
      Verbs::Associate,
      Verbs::Catenate,
      Verbs::Select,
      Verbs::Interpret
   >;


   /// File reader stream                                                     
   struct Reader final : Things::File::Reader {
      Reader(File*);

      size_t Read(Bytes&) override;
   };


   /// File writer stream                                                     
   struct Writer final : Things::File::Writer {
      Writer(File*, bool append);

      size_t Write(Bytes const&) override;
   };

protected:
   friend struct Reader;
   friend struct Writer;

   // Information about the file, if file exists                        
   PHYSFS_Stat mFileInfo {};
   // Opened file handle                                                
   mutable Own<PHYSFS_File*> mHandle;

public:
   File(FileSystem*, Many const&);
  ~File();

   void Refresh() override {}
   void Teardown();

   void Associate(Verb&);
   void Catenate(Verb&);
   void Select(Verb&);
   void Interpret(Verb&);

   auto ReadAs        (DMeta)       const -> Many override;
   auto RelativeFile  (Path const&) const -> Ref<Things::File> override;
   auto RelativeFolder(Path const&) const -> Ref<Things::Folder> override;

   auto NewReader()                 const -> Ref<Things::File::Reader> override;
   auto NewWriter(bool append)      const -> Ref<Things::File::Writer> override;
};