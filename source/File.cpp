///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "File.hpp"
#include "FileSystem.hpp"


/// File constructor                                                          
///   @param producer the file producer                                       
///   @param descriptor instructions for configuring the GUI                  
File::File(FileSystem* producer, Many const& descriptor)
   : Downcastable {this}
   , ProducedFrom {producer, descriptor} {
   VERBOSE_VFS("Initializing...");

   // Get a path from the descriptor                                    
   if (not descriptor.ExtractTag<Tags::Name, Tags::Path>(mFilePath))
      descriptor.ExtractDataAs(mFilePath);
   LglsAssert(mFilePath, "Can't interface empty file path");

   mParentDirectory = mFilePath.GetDirectory();
   mFileName = mFilePath.GetFilename();
   mFileExtension = mFilePath.GetExtension();

   // Check if file exists, and retrieve its info                       
   if (0 != PHYSFS_stat(mFilePath.GetRaw(), &mFileInfo)) {
      LglsAssert(
         mFileInfo.filetype == PHYSFS_FILETYPE_REGULAR,
         "Path `", mFilePath, "` doesn't point to a regular file"
      );

      mExists = true;
      mByteCount = static_cast<size_t>(mFileInfo.filesize);
      mIsReadOnly = mFileInfo.readonly;
      VERBOSE_VFS("Interfaces existing file: ", mFilePath);
   }
   else {
      VERBOSE_VFS("Interfaces non-existing file: ", mFilePath);
   }

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   // Check the file format, if available                               
   auto& candidates = RTTI::Registry::ResolveFileExtension(mFileExtension);
   switch(candidates.size()) {
   case 0:
      Logger::Warning(this, ": Unknown file extension: `", mFilePath, '`');
      break;
   case 1:
      mFormat = *candidates.begin();
      VERBOSE_VFS("File format detected: ", mFormat);
      if (not mFormat)
         Logger::Warning(this, ": File extension isn't associated with data: `", mFilePath, '`');
      break;
   default:
      Logger::Warning(this, ": Ambiguous file extension for `", mFilePath,
         "`, could be any of the following: ");
      for (auto ext : candidates)
         Logger::Warning("  * ", DMeta {ext});
   }
#endif

   Couple(descriptor);
   VERBOSE_VFS("Initialized");
}

/// File destructor                                                           
File::~File() {
   if (mHandle) {
      LglsAssert(PHYSFS_close(*mHandle),  "Error in PHYSFS_close: ", GetLastError());
      *mHandle = nullptr;
   }
}

/// First stage destruction                                                   
void File::Teardown() {
   mFilePath.Reset();
   mParentDirectory = {};
   mFileName = {};
   mFileExtension = {};
}

/// Read a file and deserialize it as the required type                       
Many File::ReadAs(DMeta) const {
   TODO();
   return {};
}

/// Create a new file reader                                                  
///   @return a pointer to the file reader                                    
auto File::NewReader() const -> Ref<Things::File::Reader> {
   LglsAssert(not mHandle, "File `", mFilePath, "` is already opened");

   // Open file for reading                                             
   mHandle = PHYSFS_openRead(GetFilePath().GetRaw());
   LglsAssert(mHandle, "Can't open `", GetFilePath(), "` for reading");

   Ref<::File::Reader> instance;
   instance.Emplace(this);
   return instance.As<Things::File::Reader>();
}

/// Create a new file writer                                                  
///   @return a pointer to the file writer                                    
auto File::NewWriter(bool append) const -> Ref<Things::File::Writer> {
   // Check if file is read-only                                        
   LglsAssert(
      not Exists() or not IsReadOnly(),
      "Can't open read-only `", GetFilePath(), "` for writing/appending"
   );

   LglsAssert(not mHandle, "File `", mFilePath, "` is already opened");

   if (append) {
      // Open file for appending                                        
      mHandle = PHYSFS_openAppend(GetFilePath().GetRaw());
      LglsAssert(mHandle, "Can't open `", GetFilePath(), "` for appending");
   }
   else {
      // Open file anew for writing                                     
      mHandle = PHYSFS_openWrite(GetFilePath().GetRaw());
      LglsAssert(mHandle, "Can't open `", GetFilePath(), "` for writing");
   }

   Ref<::File::Writer> instance;
   instance.Emplace(this, append);
   return instance.As<Things::File::Writer>();
}

/// Get a file interface with filename, relative to this file                 
///   @param filename path relative to this file's path                       
///   @return the file interface                                              
auto File::RelativeFile(Path const& filename) const -> Ref<Things::File> {
   return GetProducer()->GetFile(Path {mParentDirectory} / filename);
}

/// Get a subfolder interface with filename, relative to this file            
///   @param dirname path relative to this file's path                        
///   @return the folder interface                                            
auto File::RelativeFolder(Path const& dirname) const -> Ref<Things::Folder> {
   return GetProducer()->GetFolder(Path {mParentDirectory} / dirname);
}

/// Rewrite the file, by serializing the verb's arguments                     
///   @param verb the associate verb                                          
void File::Associate(Verb& verb) {
   if (verb.GetMass() <= 0)
      return;

   TODO();
}

/// Append to the file, by serializing the verb's arguments                   
///   @param verb the catenate verb                                           
void File::Catenate(Verb& verb) {
   if (verb.GetMass() <= 0)
      return;

   TODO();
}

/// Seek a position inside the file, like the front/back/specific offset      
///   @param verb the select verb                                             
void File::Select(Verb& verb) {
   if (verb.GetMass() <= 0)
      return;

   TODO();
}

/// Attempt to interpret the file contents at the current position as some    
/// data type                                                                 
///   @param verb the interpret verb                                          
void File::Interpret(Verb&) {
   TODO();
}



///                                                                           
///   File reader implementation                                              
///                                                                           

/// File reader constructor                                                   
///   @param file the file interface                                          
File::Reader::Reader(File* file)
   : Things::File::Reader {file} {}

/// Read bytes into a preallocated block                                      
///   @attention output might not be entirely filled, check return value      
///   @param output [out] the read bytes go here                              
///   @return the true number of read bytes                                   
size_t File::Reader::Read(Many& output) {
   const auto file   = mFile.As<File>();
   const auto count  = PHYSFS_uint64(output.GetBytesize());
   const auto result = PHYSFS_readBytes(*file->mHandle, output.GetRaw(), count);
   const auto r      = static_cast<size_t>(result);
   VERBOSE_VFS("Reads ", Size {r}, " from `", mFile->GetFilePath(), '`');

   LglsAssert(-1 != result, "Complete failure in PHYSFS_readBytes: ", GetLastError());
   //LglsAssert(r == count, "Error in PHYSFS_readBytes: ", GetLastError());

   mProgress += r;
   return r;
}

/*Text File::Reader::Self() const {
   return mFile ? mFile->Self() : "<invalid file reader>";
}*/



///                                                                           
///   File writer implementation                                              
///                                                                           

/// File writer constructor                                                   
///   @param file the file interface                                          
///   @param append false if you want to delete and create the file anew      
File::Writer::Writer(File* file, bool append)
   : Things::File::Writer {file, append} {}

/// Write bytes to a preallocated block                                       
///   @param input the written bytes come from here                           
///   @return the number of written bytes                                     
size_t File::Writer::Write(Many const& input) {
   const auto file   = mFile.As<::File>();
   const auto count  = PHYSFS_uint64(input.GetBytesize());
   const auto result = PHYSFS_writeBytes(*file->mHandle, input.GetRaw(), count);

   VERBOSE_VFS("Writes ", result, " to `", mFile->GetFilePath(), '`');
   LglsAssert(PHYSFS_uint64(result) == count,
      "Error in PHYSFS_writeBytes: ", GetLastError());

   const auto r = static_cast<size_t>(result);
   mProgress += r;
   return r;
}

/*Text File::Writer::Self() const {
   return mFile ? mFile->Self() : "<invalid file writer>";
}*/
