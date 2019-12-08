# FourAxisManipulator

## 2019/12/08

### Finish transport a Chinese string from `Qt widget` to `Ros` stream then to `Freetype` demo

### Solve the problem that the newest text carry the before one.
<br>
According to the offical doucuments:
<br>
  /**************************************************************************
  <br>   *
   <br>   * @type:
   <br>   *   FT_Face
   <br>   *
   <br>   * @description:
   <br>   *   A handle to a typographic face object.  A face object models a given
   <br>   *   typeface, in a given style.
   <br>   *
   <br>   * @note:
   <br>   *   A face object also owns a single @FT_GlyphSlot object, as well as one
   <br>   *   or more @FT_Size objects.
   <br>   *
   <br>   *   Use @FT_New_Face or @FT_Open_Face to create a new face object from a
   <br>   *   given filepath or a custom input stream.
   <br>   *
   <br>   *   Use @FT_Done_Face to destroy it (along with its slot and sizes).
   <br>   *
   <br>   *   An `FT_Face` object can only be safely used from one thread at a time.
   <br>   *   Similarly, creation and destruction of `FT_Face` with the same
   <br>   *   @FT_Library object can only be done from one thread at a time.  On the
   <br>   *   other hand, functions like @FT_Load_Glyph and its siblings are
   <br>   *   thread-safe and do not need the lock to be held as long as the same
   <br>   *   `FT_Face` object is not used from multiple threads at the same time.
   <br>   *
   <br>   * @also:
   <br>   *   See @FT_FaceRec for the publicly accessible fields of a given face
   <br>   *   object.
   <br>  */
<br>
So when in the case you frequently use the function, destory the face use "FT_Done_Face(face)" and make sure the image buffer is cleared to all zero.
