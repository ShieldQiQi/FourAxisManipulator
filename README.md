# FourAxisManipulator

## 2019/12/08

### Finish transport a Chinese string from `Qt widget` to `Ros` stream then to `Freetype` demo

### Solve the problem that the newest text carry the before one.
<br>
According to the offical doucuments:
<br>
>@type:
FT_Face
@description:
A handle to a typographic face object.  A face object models a given
typeface, in a given style.
   @note:
   A face object also owns a single @FT_GlyphSlot object, as well as one
   or more @FT_Size objects.
   Use @FT_New_Face or @FT_Open_Face to create a new face object from a
   given filepath or a custom input stream.
   Use @FT_Done_Face to destroy it (along with its slot and sizes). 
   An `FT_Face` object can only be safely used from one thread at a time.
   Similarly, creation and destruction of `FT_Face` with the same
  @FT_Library object can only be done from one thread at a time.  On the
   other hand, functions like @FT_Load_Glyph and its siblings are
   thread-safe and do not need the lock to be held as long as the same
  `FT_Face` object is not used from multiple threads at the same time.
   @also:
   See @FT_FaceRec for the publicly accessible fields of a given face
   object.
<br>
So when in the case you frequently use the function, destory the face use "FT_Done_Face(face)" and make sure the image buffer is cleared to all zero.
