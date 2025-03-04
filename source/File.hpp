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
#include <Langulus/Verbs/Associate.hpp>
#include <Langulus/Verbs/Catenate.hpp>
#include <Langulus/Verbs/Select.hpp>
#include <Langulus/Verbs/Interpret.hpp>
#include <optional>


///                                                                           
///   A file                                                                  
///                                                                           
struct File final : A::File, Flow::ProducedFrom<FileSystem> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) FileSystem;
   LANGULUS_BASES(A::File);
   LANGULUS_VERBS(
      Verbs::Associate,
      Verbs::Catenate,
      Verbs::Select,
      Verbs::Interpret
   );


   ///                                                                        
   /// File reader stream                                                     
   struct Reader final : A::File::Reader {
   private:
      Text Self() const;

   public:
      Reader(File*);

      Offset Read(Many&);
   };


   ///                                                                        
   /// File writer stream                                                     
   struct Writer final : A::File::Writer {
   private:
      Text Self() const;

   public:
      Writer(File*, bool append);

      Offset Write(const Many&);
   };

protected:
   friend struct Reader;
   friend struct Writer;

   // Information about the file, if file exists                        
   PHYSFS_Stat mFileInfo {};
   // Opened file handle                                                
   mutable Own<PHYSFS_File*> mHandle;

public:
   File(FileSystem*, const Many&);
  ~File();

   void Refresh() {}
   void Teardown();

   void Associate(Verb&);
   void Catenate(Verb&);
   void Select(Verb&);
   void Interpret(Verb&);

   Many ReadAs(DMeta) const;

   auto NewReader()                 const -> Ref<A::File::Reader>;
   auto NewWriter(bool append)      const -> Ref<A::File::Writer>;

   auto RelativeFile(const Path&)   const -> Ref<A::File>;
   auto RelativeFolder(const Path&) const -> Ref<A::Folder>;
};