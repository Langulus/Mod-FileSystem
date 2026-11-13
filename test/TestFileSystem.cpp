///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/CppAPI/IO.hpp>
#include <Langulus/Testing.hpp>
#include <Langulus/Allocator.hpp>

using namespace Langulus;
using Things::Thing;


SCENARIO("Non-existing file/folder interfacing") {
   static MemoryState memoryState;

   GIVEN("Init and shutdown cycles") {
      for (int repeat = 0; repeat != 10; ++repeat) {
         // Create root entity                                          
         auto root = Thing::Root<false>("FileSystem");

         REQUIRE(root.GetRuntime()->GetWorkingPath());
         REQUIRE(root.GetRuntime()->GetDataPath());

         WHEN("The file/folder is created via abstractions") {
            auto producedFile = root.CreatePart<Things::File>("nonexistent.txt");
            auto producedFold = root.CreatePart<Things::Folder>("nonexistent folder");

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(producedFile.GetCount() == 1);
            REQUIRE(producedFile.CastsTo<Things::File>(1));
            REQUIRE(producedFile.IsSparse());
            REQUIRE_FALSE(producedFile.template As<Things::File*>()->Exists());
            REQUIRE(producedFile.template As<Things::File*>()->GetFormat() == MetaDataOf<Text>());

            REQUIRE(producedFold.GetCount() == 1);
            REQUIRE(producedFold.CastsTo<Things::Folder>(1));
            REQUIRE(producedFold.IsSparse());
            REQUIRE_FALSE(producedFold.template As<Things::Folder*>()->Exists());

            REQUIRE(root.GetParts().GetCount() == 2);
         }
         
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The file/folder is created via tokens") {
            auto producedFile = root.CreatePartToken("File", "nonexistent.txt");
            auto producedFold = root.CreatePartToken("Folder", "nonexistent folder");
            
            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(producedFile.GetCount() == 1);
            REQUIRE(producedFile.CastsTo<Things::File>(1));
            REQUIRE(producedFile.IsSparse());
            REQUIRE_FALSE(producedFile.template As<Things::File*>()->Exists());
            REQUIRE(producedFile.template As<Things::File*>()->GetFormat() == MetaDataOf<Text>());

            REQUIRE(producedFold.GetCount() == 1);
            REQUIRE(producedFold.CastsTo<Things::Folder>(1));
            REQUIRE(producedFold.IsSparse());
            REQUIRE_FALSE(producedFold.template As<Things::Folder*>()->Exists());

            REQUIRE(root.GetParts().GetCount() == 2);
         }
      #endif

         REQUIRE(memoryState.Assert());
         REQUIRE_FALSE(Allocator::CollectGarbage());
      }
   }
}

SCENARIO("Existing file/folder interfacing") {
   static MemoryState memoryState;

   GIVEN("Init and shutdown cycles") {
      for (int repeat = 0; repeat != 10; ++repeat) {
         // Create root entity                                          
         auto root = Thing::Root<false>("FileSystem");

         REQUIRE(root.GetRuntime()->GetWorkingPath());
         REQUIRE(root.GetRuntime()->GetDataPath());

         WHEN("The file/folder is created via abstractions") {
            auto producedFile = root.CreatePart<Things::File>("test.txt");
            auto producedFold = root.CreatePart<Things::Folder>("test folder");

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(producedFile.GetCount() == 1);
            REQUIRE(producedFile.CastsTo<Things::File>(1));
            REQUIRE(producedFile.IsSparse());
            REQUIRE(producedFile.template As<Things::File*>()->Exists());
            REQUIRE(producedFile.template As<Things::File*>()->GetFormat() == MetaDataOf<Text>());

            REQUIRE(producedFold.GetCount() == 1);
            REQUIRE(producedFold.CastsTo<Things::Folder>(1));
            REQUIRE(producedFold.IsSparse());
            REQUIRE(producedFold.template As<Things::Folder*>()->Exists());

            REQUIRE(root.GetParts().GetCount() == 2);
         }
         
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The file/folder is created via tokens") {
            auto producedFile = root.CreatePartToken("File", "test.txt");
            auto producedFold = root.CreatePartToken("Folder", "test folder");
            
            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(producedFile.GetCount() == 1);
            REQUIRE(producedFile.CastsTo<Things::File>(1));
            REQUIRE(producedFile.IsSparse());
            REQUIRE(producedFile.template As<Things::File*>()->Exists());
            REQUIRE(producedFile.template As<Things::File*>()->GetFormat() == MetaDataOf<Text>());

            REQUIRE(producedFold.GetCount() == 1);
            REQUIRE(producedFold.CastsTo<Things::Folder>(1));
            REQUIRE(producedFold.IsSparse());
            REQUIRE(producedFold.template As<Things::Folder*>()->Exists());

            REQUIRE(root.GetParts().GetCount() == 2);
         }
      #endif

         REQUIRE(memoryState.Assert());
         REQUIRE_FALSE(Allocator::CollectGarbage());
      }
   }
}

