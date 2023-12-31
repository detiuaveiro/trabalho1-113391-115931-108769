/// image8bit - A simple image processing module.
///
/// This module is part of a programming project
/// for the course AED, DETI / UA.PT
///
/// You may freely use and modify this code, at your own risk,
/// as long as you give proper credit to the original and subsequent authors.
///
/// João Manuel Rodrigues <jmr@ua.pt>
/// 2013, 2023

// Student authors (fill in below):
// NMec: 115931 Name: Joaquim Martins
// NMec: 115931 Name: David Pelicano
// NMec: 115931 Name: Tiago Soure
// 
// Date:
//

#include "image8bit.h"
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "instrumentation.h"

// The data structure
//
// An image is stored in a structure containing 3 fields:
// Two integers store the image width and height.
// The other field is a pointer to an array that stores the 8-bit gray
// level of each pixel in the image.  The pixel array is one-dimensional
// and corresponds to a "raster scan" of the image from left to right,
// top to bottom.
// For example, in a 100-pixel wide image (img->width == 100),
//   pixel position (x,y) = (33,0) is stored in img->pixel[33];
//   pixel position (x,y) = (22,1) is stored in img->pixel[122].
// 
// Clients should use images only through variables of type Image,
// which are pointers to the image structure, and should not access the
// structure fields directly.

// Maximum value you can store in a pixel (maximum maxval accepted)
const uint8 PixMax = 255;

// Internal structure for storing 8-bit graymap images
struct image {
  int width;
  int height;
  int maxval;   // maximum gray value (pixels with maxval are pure WHITE)
  uint8* pixel; // pixel data (a raster scan)
};


// This module follows "design-by-contract" principles.
// Read `Design-by-Contract.md` for more details.

/// Error handling functions

// In this module, only functions dealing with memory allocation or file
// (I/O) operations use defensive techniques.
// 
// When one of these functions fails, it signals this by returning an error
// value such as NULL or 0 (see function documentation), and sets an internal
// variable (errCause) to a string indicating the failure cause.
// The errno global variable thoroughly used in the standard library is
// carefully preserved and propagated, and clients can use it together with
// the ImageErrMsg() function to produce informative error messages.
// The use of the GNU standard library error() function is recommended for
// this purpose.
//
// Additional information:  man 3 errno;  man 3 error;

// Variable to preserve errno temporarily
static int errsave = 0;

// Error cause
static char* errCause;
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

/// Error cause.
/// After some other module function fails (and returns an error code),
/// calling this function retrieves an appropriate message describing the
/// failure cause.  This may be used together with global variable errno
/// to produce informative error messages (using error(), for instance).
///
/// After a successful operation, the result is not garanteed (it might be
/// the previous error cause).  It is not meant to be used in that situation!
char* ImageErrMsg() { ///
  return errCause;
}


// Defensive programming aids
//
// Proper defensive programming in C, which lacks an exception mechanism,
// generally leads to possibly long chains of function calls, error checking,
// cleanup code, and return statements:
//   if ( funA(x) == errorA ) { return errorX; }
//   if ( funB(x) == errorB ) { cleanupForA(); return errorY; }
//   if ( funC(x) == errorC ) { cleanupForB(); cleanupForA(); return errorZ; }
//
// Understanding such chains is difficult, and writing them is boring, messy
// and error-prone.  Programmers tend to overlook the intricate details,
// and end up producing unsafe and sometimes incorrect programs.
//
// In this module, we try to deal with these chains using a somewhat
// unorthodox technique.  It resorts to a very simple internal function
// (check) that is used to wrap the function calls and error tests, and chain
// them into a long Boolean expression that reflects the success of the entire
// operation:
//   success = 
//   check( funA(x) != error , "MsgFailA" ) &&
//   check( funB(x) != error , "MsgFailB" ) &&
//   check( funC(x) != error , "MsgFailC" ) ;
//   if (!success) {
//     conditionalCleanupCode();
//   }
//   return success;
// 
// When a function fails, the chain is interrupted, thanks to the
// short-circuit && operator, and execution jumps to the cleanup code.
// Meanwhile, check() set errCause to an appropriate message.
// 
// This technique has some legibility issues and is not always applicable,
// but it is quite concise, and concentrates cleanup code in a single place.
// 
// See example utilization in ImageLoad and ImageSave.
//
// (You are not required to use this in your code!)


// Check a condition and set errCause to failmsg in case of failure.
// This may be used to chain a sequence of operations and verify its success.
// Propagates the condition.
// Preserves global errno!
static int check(int condition, const char* failmsg) {
  errCause = (char*)(condition ? "" : failmsg);
  return condition;
}


/// Init Image library.  (Call once!)
/// Currently, simply calibrate instrumentation and set names of counters.
void ImageInit(void) { ///
  InstrCalibrate();
  InstrName[0] = "pixmem";  // InstrCount[0] will count pixel array acesses
  // Name other counters here...
  InstrName[1] = "comp";
  InstrName[2] = "multiplicacoes";
  InstrName[3] = "somas";
  InstrCalibrate();
  
}

// Macros to simplify accessing instrumentation counters:
#define PIXMEM InstrCount[0]
// Add more macros here...

// TIP: Search for PIXMEM or InstrCount to see where it is incremented!


/// Image management functions

/// Create a new black image.
///   width, height : the dimensions of the new image.
///   maxval: the maximum gray level (corresponding to white).
/// Requires: width and height must be non-negative, maxval > 0.
/// 
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageCreate(int width, int height, uint8 maxval) { ///
  assert (width >= 0);
  assert (height >= 0);
  assert (0 < maxval && maxval <= PixMax);
  // Insert your code here!

  // Aloca memória para a estrutura da imagem
  Image img = (Image)malloc(sizeof(struct image));
  if (img == NULL) {
    errCause = "Memory allocation error";
    return NULL;
  }

  // Inicializa os campos da estrutura da imagem
  img->width = width;
  img->height = height;
  img->maxval = maxval;

  // Aloca memória para o array de pixels
  img->pixel = (uint8*)malloc(width * height * sizeof(uint8));
  if (img->pixel == NULL) {
    errCause = "Memory allocation error";
    free(img);  // Libera a memória alocada para a estrutura da imagem
    return NULL;
  }

  // Inicializa todos os pixels com 0 (preto)
  for (int i = 0; i < width * height; i++) {
        img->pixel[i] = 0;
  }
 

  return img;
}

/// Destroy the image pointed to by (*imgp).
///   imgp : address of an Image variable.
/// If (*imgp)==NULL, no operation is performed.
/// Ensures: (*imgp)==NULL.
/// Should never fail, and should preserve global errno/errCause.
void ImageDestroy(Image* imgp) { ///
  assert (imgp != NULL);
  // Insert your code here!

  // Verifica se o ponteiro para a imagem não é nulo
  if (*imgp != NULL) {
    // Libera a memória alocada para o array de pixels
    free((*imgp)->pixel);
    // Libera a memória alocada para a estrutura da imagem
    free(*imgp);
    // Define o ponteiro como nulo para evitar uso após a liberação
    *imgp = NULL;
  }

}


/// PGM file operations

// See also:
// PGM format specification: http://netpbm.sourceforge.net/doc/pgm.html

// Match and skip 0 or more comment lines in file f.
// Comments start with a # and continue until the end-of-line, inclusive.
// Returns the number of comments skipped.
static int skipComments(FILE* f) {
  char c;
  int i = 0;
  while (fscanf(f, "#%*[^\n]%c", &c) == 1 && c == '\n') {
    i++;
  }
  return i;
}

/// Load a raw PGM file.
/// Only 8 bit PGM files are accepted.
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageLoad(const char* filename) { ///
  int w, h;
  int maxval;
  char c;
  FILE* f = NULL;
  Image img = NULL;

  int success = 
  check( (f = fopen(filename, "rb")) != NULL, "Open failed" ) &&
  // Parse PGM header
  check( fscanf(f, "P%c ", &c) == 1 && c == '5' , "Invalid file format" ) &&
  skipComments(f) >= 0 &&
  check( fscanf(f, "%d ", &w) == 1 && w >= 0 , "Invalid width" ) &&
  skipComments(f) >= 0 &&
  check( fscanf(f, "%d ", &h) == 1 && h >= 0 , "Invalid height" ) &&
  skipComments(f) >= 0 &&
  check( fscanf(f, "%d", &maxval) == 1 && 0 < maxval && maxval <= (int)PixMax , "Invalid maxval" ) &&
  check( fscanf(f, "%c", &c) == 1 && isspace(c) , "Whitespace expected" ) &&
  // Allocate image
  (img = ImageCreate(w, h, (uint8)maxval)) != NULL &&
  // Read pixels
  check( fread(img->pixel, sizeof(uint8), w*h, f) == w*h , "Reading pixels" );
  PIXMEM += (unsigned long)(w*h);  // count pixel memory accesses

  // Cleanup
  if (!success) {
    errsave = errno;
    ImageDestroy(&img);
    errno = errsave;
  }
  if (f != NULL) fclose(f);
  return img;
}

/// Save image to PGM file.
/// On success, returns nonzero.
/// On failure, returns 0, errno/errCause are set appropriately, and
/// a partial and invalid file may be left in the system.
int ImageSave(Image img, const char* filename) { ///
  assert (img != NULL);
  int w = img->width;
  int h = img->height;
  uint8 maxval = img->maxval;
  FILE* f = NULL;

  int success =
  check( (f = fopen(filename, "wb")) != NULL, "Open failed" ) &&
  check( fprintf(f, "P5\n%d %d\n%u\n", w, h, maxval) > 0, "Writing header failed" ) &&
  check( fwrite(img->pixel, sizeof(uint8), w*h, f) == w*h, "Writing pixels failed" ); 
  PIXMEM += (unsigned long)(w*h);  // count pixel memory accesses

  // Cleanup
  if (f != NULL) fclose(f);
  return success;
}


/// Information queries

/// These functions do not modify the image and never fail.

/// Get image width
int ImageWidth(Image img) { ///
  assert (img != NULL);
  return img->width;
}

/// Get image height
int ImageHeight(Image img) { ///
  assert (img != NULL);
  return img->height;
}

/// Get image maximum gray level
int ImageMaxval(Image img) { ///
  assert (img != NULL);
  return img->maxval;
}

/// Pixel stats
/// Find the minimum and maximum gray levels in image.
/// On return,
/// *min is set to the minimum gray level in the image,
/// *max is set to the maximum.
void ImageStats(Image img, uint8* min, uint8* max) { ///
  assert (img != NULL);
  // Insert your code here!

  assert(min != NULL);
  assert(max != NULL);

  // Inicializa os valores mínimos e máximos com os extremos possíveis
  *min = PixMax;
  *max = 0;

  // Percorre todos os pixels para encontrar os níveis mínimo e máximo
  for (int y = 0; y < img->height; y++) {
    for (int x = 0; x < img->width; x++) {
      uint8 pixelValue = ImageGetPixel(img, x, y);
      if (pixelValue < *min) {
        *min = pixelValue;
      }
      if (pixelValue > *max) {
        *max = pixelValue;
      }
    }
  }
}

/// Check if pixel position (x,y) is inside img.
int ImageValidPos(Image img, int x, int y) {
  assert(img != NULL);

  // Verifica se x está dentro dos limites da imagem
  if (x < 0 || x >= img->width) {
    return 0;
  }

  // Verifica se y está dentro dos limites da imagem
  if (y < 0 || y >= img->height) {
    return 0;
  }

  // Se ambas as verificações passarem, a posição está dentro da imagem
  return 1;
}

/// Check if rectangular area (x,y,w,h) is completely inside img.
int ImageValidRect(Image img, int x, int y, int w, int h) { ///
  assert (img != NULL);
  // Insert your code here!

  // Verifica se a posição inicial (x, y) é válida
  if (!ImageValidPos(img, x, y)) {
    return 0;
  }

  // Verifica se a posição final (x + w, y + h) é válida
  if (!ImageValidPos(img, x + w - 1, y + h - 1)) {
    return 0;
  }

  // Se ambas as verificações passarem, a área está completamente dentro da imagem
  return 1;

}

/// Pixel get & set operations

/// These are the primitive operations to access and modify a single pixel
/// in the image.
/// These are very simple, but fundamental operations, which may be used to 
/// implement more complex operations.

// Transform (x, y) coords into linear pixel index.
// This internal function is used in ImageGetPixel / ImageSetPixel. 
// The returned index must satisfy (0 <= index < img->width*img->height)
static inline int G(Image img, int x, int y) {
  int index;
  // Insert your code here!
  index = y * img->width + x;
  assert (0 <= index && index < img->width*img->height);
  return index;
}

/// Get the pixel (level) at position (x,y).
uint8 ImageGetPixel(Image img, int x, int y) { ///
  assert (img != NULL);
  assert (ImageValidPos(img, x, y));
  PIXMEM += 1;  // count one pixel access (read)
  return img->pixel[G(img, x, y)];
} 

/// Set the pixel at position (x,y) to new level.
void ImageSetPixel(Image img, int x, int y, uint8 level) { ///
  assert (img != NULL);
  assert (ImageValidPos(img, x, y));
  PIXMEM += 1;  // count one pixel access (store)
  img->pixel[G(img, x, y)] = level;
} 


/// Pixel transformations

/// These functions modify the pixel levels in an image, but do not change
/// pixel positions or image geometry in any way.
/// All of these functions modify the image in-place: no allocation involved.
/// They never fail.


/// Transform image to negative image.
/// This transforms dark pixels to light pixels and vice-versa,
/// resulting in a "photographic negative" effect.
void ImageNegative(Image img) { ///
  assert (img != NULL);
  // Insert your code here!
  // Obtém o valor máximo de pixel
  uint8 maxval = img->maxval;

  // Percorre todos os pixels da imagem
  for (int y = 0; y < img->height; y++) {
    for (int x = 0; x < img->width; x++) {
      // Obtém o valor do pixel
      uint8 pixelValue = ImageGetPixel(img, x, y);

      // Calcula o negativo do pixel
      uint8 negativeValue = maxval - pixelValue;

      // Define o novo valor do pixel
      ImageSetPixel(img, x, y, negativeValue);
    }
  }
}

/// Apply threshold to image.
/// Transform all pixels with level<thr to black (0) and
/// all pixels with level>=thr to white (maxval).
void ImageThreshold(Image img, uint8 thr) { ///
  assert (img != NULL);
  // Insert your code here!
  // Obtém o valor máximo de pixel
  uint8 maxval = img->maxval;

  // Percorre todos os pixels da imagem
  for (int y = 0; y < img->height; y++) {
    for (int x = 0; x < img->width; x++) {
      // Obtém o valor do pixel
      uint8 pixelValue = ImageGetPixel(img, x, y);

      // Aplica a condição de limiar
      uint8 newValue = (pixelValue < thr) ? 0 : maxval;

      // Define o novo valor do pixel
      ImageSetPixel(img, x, y, newValue);
    }
  }
}

/// dghten image by a factor.
/// Multiply each pixel level by a factor, but saturate at maxval.
/// This will brighten the image if factor>1.0 and
/// darken the image if factor<1.0.
void ImageBrighten(Image img, double factor) {
  assert(img != NULL);
  assert(factor >= 0);

  for (int y = 0; y < img->height; y++) {
    for (int x = 0; x < img->width; x++) {
      // Obtenha o valor do pixel atual
      int pixel = ImageGetPixel(img, x, y);

      // Ajuste o valor do pixel
      pixel = (int)round(pixel * factor);

      // Certifique-se de que o pixel está dentro dos limites de 8 bits
      if (pixel > 255) {
        pixel = 255;
      } else if (pixel < 0) {
        pixel = 0;
      }

      // Defina o novo valor do pixel
      ImageSetPixel(img, x, y, pixel);
    }
  }
}


/// Geometric transformations

/// These functions apply geometric transformations to an image,
/// returning a new image as a result.
/// 
/// Success and failure are treated as in ImageCreate:
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.

// Implementation hint: 
// Call ImageCreate whenever you need a new image!

/// Rotate an image.
/// Returns a rotated version of the image.
/// The rotation is 90 degrees anti-clockwise.
/// Ensures: The original img is not modified.
/// 
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageRotate(Image img) { ///
  assert (img != NULL);
  // Insert your code here!

  // Obter as dimensões da imagem original
  int width = img->height;  // Trocar largura e altura para rotação de 90 graus
  int height = img->width;

  // Criar uma nova imagem com as dimensões trocadas
  Image rotatedImg = ImageCreate(width, height, img->maxval);

  // Percorrer todos os pixels da imagem original e copiar para a imagem rotacionada
  for (int y = 0; y < img->height; y++) {
    for (int x = 0; x < img->width; x++) {
      // Calcular as coordenadas na imagem rotacionada
      int rotatedX = y;           // Trocar coordenadas x e y
      int rotatedY = img->width - x - 1;

      // Obter o valor do pixel na imagem original
      uint8 pixelValue = ImageGetPixel(img, x, y);

      // Definir o valor do pixel na imagem rotacionada
      ImageSetPixel(rotatedImg, rotatedX, rotatedY, pixelValue);
    }
  }

  return rotatedImg;
}

/// Mirror an image = flip left-right.
/// Returns a mirrored version of the image.
/// Ensures: The original img is not modified.
/// 
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageMirror(Image img) { ///
  assert (img != NULL);
  // Insert your code here!

  // Obter as dimensões da imagem original
  int width = img->width;
  int height = img->height;

  // Criar uma nova imagem espelhada com as mesmas dimensões
  Image mirroredImg = ImageCreate(width, height, img->maxval);

  // Percorrer todos os pixels da imagem original e copiar para a imagem espelhada
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      // Calcular as coordenadas na imagem espelhada
      int mirroredX = width - x - 1;
      int mirroredY = y;

      // Obter o valor do pixel na imagem original
      uint8 pixelValue = ImageGetPixel(img, x, y);

      // Definir o valor do pixel na imagem espelhada
      ImageSetPixel(mirroredImg, mirroredX, mirroredY, pixelValue);
    }
  }

  return mirroredImg;
}

/// Crop a rectangular subimage from img.
/// The rectangle is specified by the top left corner coords (x, y) and
/// width w and height h.
/// Requires:
///   The rectangle must be inside the original image.
/// Ensures:
///   The original img is not modified.
///   The returned image has width w and height h.
/// 
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageCrop(Image img, int x, int y, int w, int h) { ///
  assert (img != NULL);
  assert (ImageValidRect(img, x, y, w, h));
  // Insert your code here!

  // Criar uma nova imagem com as dimensões especificadas
  Image croppedImg = ImageCreate(w, h, img->maxval);

  // Percorrer os pixels da região de recorte e copiar para a nova imagem
  for (int cropY = 0; cropY < h; cropY++) {
    for (int cropX = 0; cropX < w; cropX++) {
      // Obter as coordenadas na imagem original
      int originalX = x + cropX;
      int originalY = y + cropY;

      // Obter o valor do pixel na imagem original
      uint8 pixelValue = ImageGetPixel(img, originalX, originalY);

      // Definir o valor do pixel na imagem recortada
      ImageSetPixel(croppedImg, cropX, cropY, pixelValue);
    }
  }

  return croppedImg;
}


/// Operations on two images

/// Paste an image into a larger image.
/// Paste img2 into position (x, y) of img1.
/// This modifies img1 in-place: no allocation involved.
/// Requires: img2 must fit inside img1 at position (x, y).
void ImagePaste(Image img1, int x, int y, Image img2) { ///
  assert (img1 != NULL);
  assert (img2 != NULL);
  assert (ImageValidRect(img1, x, y, img2->width, img2->height));
  // Insert your code here!

  // Percorrer os pixels da imagem img2
  for (int srcY = 0; srcY < img2->height; srcY++) {
    for (int srcX = 0; srcX < img2->width; srcX++) {
      // Calcular as coordenadas na imagem img1
      int destX = x + srcX;
      int destY = y + srcY;

      // Obter o valor do pixel na imagem img2
      uint8 pixelValue = ImageGetPixel(img2, srcX, srcY);

      // Definir o valor do pixel na imagem img1
      ImageSetPixel(img1, destX, destY, pixelValue);
    }
  }
  if (img1->width >= img2->width && img1->height >= img2->height) {
        printf("Dá para fazer paste");
  } else {
        printf("A imagem2 é muito grande para ser colada na imagem1.\n");
  }

}

/// Blend an image into a larger image.
/// Blend img2 into position (x, y) of img1.
/// This modifies img1 in-place: no allocation involved.
/// Requires: img2 must fit inside img1 at position (x, y).
/// alpha usually is in [0.0, 1.0], but values outside that interval
/// may provide interesting effects.  Over/underflows should saturate.
void ImageBlend(Image img1, int x, int y, Image img2, double alpha) { ///
  assert (img1 != NULL);
  assert (img2 != NULL);
  assert (ImageValidRect(img1, x, y, img2->width, img2->height));
  // Insert your code here!

  // Percorrer os pixels da imagem img2
  for (int srcY = 0; srcY < img2->height; srcY++) {
    for (int srcX = 0; srcX < img2->width; srcX++) {
      // Calcular as coordenadas na imagem img1
      int destX = x + srcX;
      int destY = y + srcY;

      // Obter o valor do pixel na imagem img2
      uint8 pixelValue2 = ImageGetPixel(img2, srcX, srcY);

      // Obter o valor do pixel na imagem img1
      uint8 pixelValue1 = ImageGetPixel(img1, destX, destY);

      // Calcular o novo valor do pixel com a mistura ponderada
      uint8 blendedValue = (uint8)round((1.0 - alpha) * pixelValue1 + alpha * pixelValue2);

      // Definir o valor do pixel na imagem img1
      ImageSetPixel(img1, destX, destY, blendedValue);
    }
  }
}

/// Compare an image to a subimage of a larger image.
/// Returns 1 (true) if img2 matches subimage of img1 at pos (x, y).
/// Returns 0, otherwise.
int ImageMatchSubImage(Image img1, int x, int y, Image img2) { ///
  assert (img1 != NULL);
  assert (img2 != NULL);
  assert (ImageValidPos(img1, x, y));
  // Insert your code here!

  // Verificar se a subimagem img2 cabe dentro da imagem img1 na posição (x, y)
  if (!ImageValidRect(img1, x, y, img2->width, img2->height)) {
    return 0;  // A subimagem não cabe, retorno falso
  }

  // Comparar pixels correspondentes
  for (int srcY = 0; srcY < img2->height; srcY++) {
    for (int srcX = 0; srcX < img2->width; srcX++) {
      // Coordenadas na imagem img1
      int destX = x + srcX;
      int destY = y + srcY;

      // Obter valores dos pixels nas duas imagens
      uint8 pixelValue1 = ImageGetPixel(img1, destX, destY);
      uint8 pixelValue2 = ImageGetPixel(img2, srcX, srcY);


      COMP++;
      // Verificar se os pixels correspondentes são diferentes
      if (pixelValue1 != pixelValue2) {
        return 0;  // Não há correspondência, retorno falso
      }
    }
  }

  // Todos os pixels correspondentes são iguais, retorno verdadeiro
  return 1;
}

/// Locate a subimage inside another image.
/// Searches for img2 inside img1.
/// If a match is found, returns 1 and matching position is set in vars (*px, *py).
/// If no match is found, returns 0 and (*px, *py) are left untouched.
int ImageLocateSubImage(Image img1, int* px, int* py, Image img2) { ///
  assert (img1 != NULL);
  assert (img2 != NULL);
  // Insert your code here!

  // Percorrer todas as posições possíveis em img1
  for (int y = 0; y <= img1->height - img2->height; y++) {
    for (int x = 0; x <= img1->width - img2->width; x++) {
      // Verificar se há uma correspondência na posição atual
      if (ImageMatchSubImage(img1, x, y, img2)) {
        // Correspondência encontrada, definir as posições e retornar 1
        *px = x;
        *py = y;

        return 1;
      }
      
    }
  }

  // Nenhuma correspondência encontrada, deixar *px e *py inalterados e retornar 0
  return 0;
}


/// Filtering

/// Blur an image by a applying a (2dx+1)x(2dy+1) mean filter.
/// Each pixel is substituted by the mean of the pixels in the rectangle
/// [x-dx, x+dx]x[y-dy, y+dy].
/// The image is changed in-place.
void ImageBlur(Image img, int dx, int dy) {
  assert(img != NULL);
  assert(img->width >= 0 && img->height >= 0);
  InstrReset();

  // Create a temporary copy of the image to preserve the original values
  Image tempImg = ImageCreate(img->width, img->height, img->maxval);
  if (tempImg == NULL) {
    // Error handling if allocation fails
    return;
  }

  // Traverse all pixels of the image
  for (int y = 0; y < img->height; y++) {
    for (int x = 0; x < img->width; x++) {
      int sum = 0;
      int count = 0;

      // Traverse the window around the current pixel
      for (int j = -dy; j <= dy; j++) {
        for (int i = -dx; i <= dx; i++) {


          // Check if the position is valid
          if (ImageValidPos(img, x + i, y + j)) {
            SOMAS += 2;
            // Sum the values of pixels in the window
            sum += ImageGetPixel(img, x + i, y + j);
            InstrCount[0] += 3;  // to count array acesses
            InstrCount[1] += 1;  // to count addition
            count++;
          }
        }
      }

      // Calculate the average of the values in the window and set the new pixel value
      //int average = (count > 0) ? sum / (double)count : 0;
      uint8 average = (uint8)round(sum/(double)count);
      MULTIPLICACOES ++;
      ImageSetPixel(tempImg, x, y, average);
    }
  }

  // Copy the values from the temporary image back to the original image
  for (int y = 0; y < img->height; y++) {
    for (int x = 0; x < img->width; x++) {
      uint8 pixel = ImageGetPixel(tempImg, x, y);
      ImageSetPixel(img, x, y, pixel);
    }
  }
  InstrPrint();
  // Free the temporary image
  ImageDestroy(&tempImg);
}

void ImageBlurv2(Image img, int dx, int dy) {
  assert(img != NULL);
  InstrReset();
  // Create an integral image
  int width = img->width;
  int height = img->height;
  int** integralImage = (int**)malloc(height * sizeof(int*)); // Aloca memória para a imagem integral
  
  for (int i = 0; i < height; i++) {
    integralImage[i] = (int*)calloc(width, sizeof(int));
    // Inicializa a imagem integral com zeros
  }

  // Calcula a imagem integral
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      integralImage[y][x] = ImageGetPixel(img, x, y); 
      // preenche a imagem integral com as somas cumulativas de pixels
      if (y > 0) integralImage[y][x] += integralImage[y - 1][x];
      SOMAS++;
      // se não estiver na primeira linha(y>0) adiciona a soma cumulativa da coluna acima do pixel atual
      if (x > 0) integralImage[y][x] += integralImage[y][x - 1];
      SOMAS++;
      // se não estiver na primeira coluna(x>0) adiciona a soma cumulativa da linha à esquerda do pixel atual
      if (y > 0 && x > 0) integralImage[y][x] -= integralImage[y - 1][x - 1];
      SOMAS++;
      // Subtrai a soma cumulativa da diagonal superior esquerda do pixel atual (se não estiver na primeira linha nem na primeira coluna)
    }
  }

  // Aplica o desfoque
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int x1 = x - dx - 1;  // Define a borda esquerda da janela de desfoque
      int x2 = x + dx; // Define a borda direita da janela de desfoque
      int y1 = y - dy - 1; // Define a borda superior da janela de desfoque
      int y2 = y + dy; // Define a borda inferior da janela de desfoque
      SOMAS += 4;
      int sum = 0; // soma dos pixels que irão desfocar
      if (x2 < width && y2 < height){
        sum += integralImage[y2][x2];// Adiciona o valor do pixel inferior direito
        SOMAS++;
      } 
      if (x1 >= 0 && y2 < height){
        sum -= integralImage[y2][x1];// Subtrai o valor do pixel inferior esquerdo
        SOMAS++;
      } 
      if (y1 >= 0 && x2 < width){
        sum -= integralImage[y1][x2];// Subtrai o valor do pixel superior direito
        SOMAS++;
      } 
      if (x1 >= 0 && y1 >= 0){
        sum += integralImage[y1][x1];// Adiciona o valor do pixel superior esquerdo
        SOMAS++;
      } 
      int x1_clamped = max(0, x1 + 1);
      int x2_clamped = min(width, x2);
      int y1_clamped = max(0, y1 + 1);
      int y2_clamped = min(height, y2);
      int count = (x2_clamped - x1_clamped) * (y2_clamped - y1_clamped);// Calcula o número de pixels na janela de desfoque
      int temp = round((double)sum / count);// Calcula a média dos pixels na janela de desfoque

      MULTIPLICACOES += 2;
      ImageSetPixel(img, x, y, temp);// Define o pixel na imagem original para a média calculada
    }
  }

  InstrPrint();
  for (int i = 0; i < height; i++) {
    free(integralImage[i]); //para cada linha da imagem integral, libera memória
  }
  free(integralImage);// Libera a memória da imagem integral
}

