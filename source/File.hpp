///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright(C) 2023 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"
#include <Flow/Verbs/Associate.hpp>
#include <Flow/Verbs/Catenate.hpp>
#include <Flow/Verbs/Select.hpp>
#include <Flow/Verbs/Interpret.hpp>
#include <optional>


///                                                                           
///   A file                                                                  
///                                                                           
struct File final : A::File, ProducedFrom<FileSystem> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) FileSystem;
   LANGULUS_BASES(A::File);
   LANGULUS_VERBS(
      Verbs::Associate,
      Verbs::Catenate,
      Verbs::Select,
      Verbs::Interpret
   );

   /// File reader stream                                                     
   struct Reader final : A::File::Reader {
   private:
      // The low-level file interface                                   
      Own<PHYSFS_File*> mHandle;

      Debug Self() const;

   public:
      Reader(File*);
      Reader(Abandoned<Reader>&&);
      ~Reader();

      Offset Read(Any&);
   };

   /// File writer stream                                                     
   struct Writer final : A::File::Writer {
   private:
      // The low-level file interface                                   
      Own<PHYSFS_File*> mHandle;

      Debug Self() const;

   public:
      Writer(File*, bool append);
      Writer(Abandoned<Writer>&&);
      ~Writer();

      Offset Write(const Any&);
   };

private:
   // Parent directory substring, mapped onto A::File::mFilePath        
   Token mParentDirectory;
   // File name after all directories, mapped onto A::File::mFilePath   
   Token mFileName;
   // The extension of the filename, mapped onto A::File::mFilePath     
   Token mFileExtension;

   // Information about the file, if file exists                        
   PHYSFS_Stat mFileInfo {};

   // Active file reader streams                                        
   TAny<Reader> mReaders;
   // Active file writer stream (there can be only one)                 
   std::optional<Writer> mWriter;

public:
   File(FileSystem*, const Neat&);

   void Refresh() {}

   void Associate(Verb&);
   void Catenate(Verb&);
   void Select(Verb&);
   void Interpret(Verb&);

   NOD() Any ReadAs(DMeta) const;

   NOD() Ptr<A::File::Reader> NewReader();
   NOD() Ptr<A::File::Writer> NewWriter(bool append);
};


///                                                                           
///   Convenience wrapper for bytes used in pretty logging                    
///                                                                           
struct ByteCount : Math::TNumber<Size, ByteCount> {
   using TNumber::TNumber;

   static constexpr Size Bits[] = {0u,
      1u, 2u, 4u, 8u, 16u, 32u, 64u, 128u, 256u,
      512u, 1024u, 2048u, 4096u, 8192u, 16384u, 32768u, 65536u,
      131072u, 262144u, 524288u, 1048576u, 2097152u, 4194304u, 8388608u, 16777216u,
      33554432u, 67108864u, 134217728u, 268435456u, 536870912u, 1073741824u, 2147483648u
   };

   static constexpr Size KB = 1024;
   static constexpr Size MB = KB * 1024;
   static constexpr Size GB = MB * 1024;
   static constexpr Size Kib = 128;
   static constexpr Size Mib = Kib * 1024;
   static constexpr Size Gib = Mib * 1024;
   
   static constexpr Double KB_d = static_cast<Double>(KB);
   static constexpr Double MB_d = static_cast<Double>(MB);
   static constexpr Double GB_d = static_cast<Double>(GB);
   static constexpr Double Kib_d = static_cast<Double>(Kib);
   static constexpr Double Mib_d = static_cast<Double>(Mib);
   static constexpr Double Gib_d = static_cast<Double>(Gib);


   /// Get the number of the first mask matching bit                          
   NOD() static constexpr Size BitIndex(const Size mask) noexcept {
      for (Size i = sizeof(Size) * 8; i > 0; --i)
         if ((mask & Bits[i]) != 0)
            return i;
      return 0;
   }

   /// Get the value of the bit with number `thebit`                          
   NOD() static constexpr Size BitMark(const Size thebit) noexcept {
      return Bits[thebit];
   }

   /// Byte count suffix generator                                            
   static constexpr Token Suffix(const Size bytes, Size& coverted, Size& fraction) noexcept {
      coverted = bytes / GB;
      if (coverted > 0) {
         fraction = static_cast<Size>((static_cast<Double>(bytes % GB) / GB_d) * 100.0);
         return " GB";
      }

      coverted = bytes / MB;
      if (coverted > 0) {
         fraction = static_cast<Size>((static_cast<Double>(bytes % MB) / MB_d) * 100.0);
         return " MB";
      }

      coverted = bytes / KB;
      if (coverted > 0) {
         fraction = static_cast<Size>((static_cast<Double>(bytes % KB) / KB_d) * 100.0);
         return " KB";
      }

      coverted = bytes;
      fraction = 0;

      if (coverted == 1)
         return " byte";
      return " bytes";
   }

   /// Bit count suffix generator                                             
   static constexpr Token SuffixBits(const Size bits, Size& coverted, Size& fraction) noexcept {
      coverted = bits / Gib;
      if (coverted > 0) {
         fraction = static_cast<Size>((static_cast<Double>(bits % Gib) / Gib_d) * 100.0);
         return " Gib";
      }

      coverted = bits / Mib;
      if (coverted > 0) {
         fraction = static_cast<Size>((static_cast<Double>(bits % Mib) / Mib_d) * 100.0);
         return " Mib";
      }

      coverted = bits / Kib;
      if (coverted > 0) {
         fraction = static_cast<Size>((static_cast<Double>(bits % Kib) / Kib_d) * 100.0);
         return " Kib";
      }

      coverted = bits;
      fraction = 0;

      if (coverted == 1)
         return " bit";
      return " bits";
   }

   /// Stringify the byte count, make it as pretty as we can                  
   operator Debug() const noexcept {
      Size converted = 0;
      Size fraction = 0;
      const auto suffix = Suffix(mValue, converted, fraction);
      while (fraction > 99)
         fraction /= 10;

      ::std::array<char, ::std::numeric_limits<Size>::digits10 * 2 + 8> text{};
      auto result = ::std::to_chars(text.data(), text.data() + text.size(), converted);
      if (result.ec != ::std::errc()) {
         text[0] = '\0';
         return text.data();
      }

      if (fraction > 0) {
         *result.ptr = '.'; ++result.ptr;
         result = ::std::to_chars(result.ptr, text.data() + text.size(), fraction);
         if (result.ec != ::std::errc()) {
            text[0] = '\0';
            return text.data();
         }
      }

      for (Size i = 0; i < suffix.size(); ++i) {
         *result.ptr = suffix[i];
         ++result.ptr;
      }

      *result.ptr = '\0';
      return text.data();
   }
};