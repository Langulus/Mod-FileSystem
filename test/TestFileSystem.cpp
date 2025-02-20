///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/IO.hpp>
#include <Langulus/Testing.hpp>


SCENARIO("Non-existing file/folder interfacing", "[filesystem]") {
   static Allocator::State memoryState;

   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         auto root = Thing::Root<false>("FileSystem");

         REQUIRE(root.GetRuntime()->GetWorkingPath());
         REQUIRE(root.GetRuntime()->GetDataPath());

         WHEN("The file/folder is created via abstractions") {
            auto producedFile = root.CreateUnit<A::File>("nonexistent.txt");
            auto producedFold = root.CreateUnit<A::Folder>("nonexistent folder");

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(producedFile.GetCount() == 1);
            REQUIRE(producedFile.CastsTo<A::File>(1));
            REQUIRE(producedFile.IsSparse());
            REQUIRE_FALSE(producedFile.template As<A::File*>()->Exists());
            REQUIRE(producedFile.template As<A::File*>()->GetFormat()->template IsExact<Text>());

            REQUIRE(producedFold.GetCount() == 1);
            REQUIRE(producedFold.CastsTo<A::Folder>(1));
            REQUIRE(producedFold.IsSparse());
            REQUIRE_FALSE(producedFold.template As<A::Folder*>()->Exists());

            REQUIRE(root.GetUnits().GetCount() == 2);
         }
         
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The file/folder is created via tokens") {
            auto producedFile = root.CreateUnitToken("File", "nonexistent.txt");
            auto producedFold = root.CreateUnitToken("Folder", "nonexistent folder");
            
            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(producedFile.GetCount() == 1);
            REQUIRE(producedFile.CastsTo<A::File>(1));
            REQUIRE(producedFile.IsSparse());
            REQUIRE_FALSE(producedFile.template As<A::File*>()->Exists());
            REQUIRE(producedFile.template As<A::File*>()->GetFormat()->template IsExact<Text>());

            REQUIRE(producedFold.GetCount() == 1);
            REQUIRE(producedFold.CastsTo<A::Folder>(1));
            REQUIRE(producedFold.IsSparse());
            REQUIRE_FALSE(producedFold.template As<A::Folder*>()->Exists());

            REQUIRE(root.GetUnits().GetCount() == 2);
         }
      #endif

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());
      }
   }
}

SCENARIO("Existing file/folder interfacing", "[filesystem]") {
   static Allocator::State memoryState;

   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         auto root = Thing::Root<false>("FileSystem");

         REQUIRE(root.GetRuntime()->GetWorkingPath());
         REQUIRE(root.GetRuntime()->GetDataPath());

         WHEN("The file/folder is created via abstractions") {
            auto producedFile = root.CreateUnit<A::File>("test.txt");
            auto producedFold = root.CreateUnit<A::Folder>("test folder");

            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(producedFile.GetCount() == 1);
            REQUIRE(producedFile.CastsTo<A::File>(1));
            REQUIRE(producedFile.IsSparse());
            REQUIRE(producedFile.template As<A::File*>()->Exists());
            REQUIRE(producedFile.template As<A::File*>()->GetFormat()->template IsExact<Text>());

            REQUIRE(producedFold.GetCount() == 1);
            REQUIRE(producedFold.CastsTo<A::Folder>(1));
            REQUIRE(producedFold.IsSparse());
            REQUIRE(producedFold.template As<A::Folder*>()->Exists());

            REQUIRE(root.GetUnits().GetCount() == 2);
         }
         
      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The file/folder is created via tokens") {
            auto producedFile = root.CreateUnitToken("File", "test.txt");
            auto producedFold = root.CreateUnitToken("Folder", "test folder");
            
            // Update once                                              
            root.Update({});
            root.DumpHierarchy();

            REQUIRE(producedFile.GetCount() == 1);
            REQUIRE(producedFile.CastsTo<A::File>(1));
            REQUIRE(producedFile.IsSparse());
            REQUIRE(producedFile.template As<A::File*>()->Exists());
            REQUIRE(producedFile.template As<A::File*>()->GetFormat()->template IsExact<Text>());

            REQUIRE(producedFold.GetCount() == 1);
            REQUIRE(producedFold.CastsTo<A::Folder>(1));
            REQUIRE(producedFold.IsSparse());
            REQUIRE(producedFold.template As<A::Folder*>()->Exists());

            REQUIRE(root.GetUnits().GetCount() == 2);
         }
      #endif

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());
      }
   }
}

