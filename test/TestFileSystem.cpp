///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright(C) 2023 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Main.hpp"
#include <Entity/Thing.hpp>
#include <catch2/catch.hpp>

/// See https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md        
CATCH_TRANSLATE_EXCEPTION(::Langulus::Exception const& ex) {
   const Text serialized {ex};
   return ::std::string {Token {serialized}};
}

SCENARIO("File/folder creation", "[gui]") {
   Allocator::State memoryState;

   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         Thing root;
         root.SetName("ROOT");

         // Create runtime at the root                                  
         root.CreateRuntime();

         // Load file system module                                     
         root.LoadMod("FileSystem");

         REQUIRE(root.GetRuntime()->GetWorkingPath());
         REQUIRE(root.GetRuntime()->GetDataPath());

         WHEN("The file/folder is created via tokens") {
            auto producedFile = root.CreateUnitToken("File", "test.txt");
            auto producedFold = root.CreateUnitToken("Folder", "test folder");
            
            // Update once                                              
            root.Update(Time::zero());

            THEN("Various traits change") {
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
            }
         }

         WHEN("The file/folder is created via abstractions") {
            auto producedFile = root.CreateUnit<A::File>("test.txt");
            auto producedFold = root.CreateUnit<A::Folder>("test folder");

            // Update once                                              
            root.Update(Time::zero());

            THEN("Various traits change") {
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
            }
         }

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());
      }
   }
}

