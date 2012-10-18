TextToPDFArchive
================

Uses Haru PDF Library (libhpdf, libharu) to turn a text file into a PDF/A document.

This is a Windows-only demo of Haru PDF (libhpdf)'s PDF/A creation. It is something that I created in passing during 
a project for Kellpro Inc that required generation of PDF/A documents and is by no means a complete program, but perhaps
it will help those trying to create PDF/A documents under Windows using only open-source software.

The VC10 directory contains project files that allow the program to be built using Visual Studio 2010. I have not
tested it under the Express Edition, but I'm not aware of any reason that it wouldn't work. These project files 
require that three open source libraries be available to link into the project. The source for libhpdf, libpng, and
zlib will need to be cloned to directories named after each respective project in the same directory as the 
TextToPdfArchive directory. In other words, your directory structure should look like this:

zlib - cloned from https://github.com/madler/zlib.git

libpng - cloned from https://github.com/LuaDist/libpng.git

libhpdf - cloned from https://github.com/libharu/libharu.git

TextToPDFArchive - cloned from https://github.com/ahenager/TextToPDFArchive.git

The syntax for the generated TextToPDFArchive.exe is as follows:
TextToPDFArchive in.txt out.pdf

Thanks to Kellpro Inc for letting me publish this on company time.

License
=======
Copyright (c) 2012 Alan Henager

Any person may use, distribute, sell, or modify this work with limitation.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Color Profile License
=====================

Included is a color profile from basiCColor GmbH that they were kind enough to open source. Below is the license 
for it.

Open Source Initiative OSI - The zlib/libpng License:Licensing
[OSI Approved License]
The zlib/libpng License

Copyright (c) 2007, basICColor GmbH

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must  
not
  claim that you wrote the original software. If you use this software
  in a product, an acknowledgment in the product documentation would be
  appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and must  
not be
  misrepresented as being the original software.

  3. This notice may not be removed or altered from any source
  distribution.