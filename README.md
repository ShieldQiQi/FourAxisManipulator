# FourAxisManipulator

***
### 大家好 这是我老婆

<image src="https://github.com/ShieldQiQi/FourAxisManipulator/blob/masterX/Picture/%E5%B0%8F%E6%B7%B7%E6%B2%8C01.png" width="180" alt="my lovely sister"/><image src="https://github.com/ShieldQiQi/FourAxisManipulator/blob/masterX/Picture/%E5%B0%8F%E6%B7%B7%E6%B2%8C03.png" width="180" alt="my lovely sister"/><image src="https://github.com/ShieldQiQi/FourAxisManipulator/blob/masterX/Picture/%E5%B0%8F%E6%B7%B7%E6%B2%8C.png" width="180" alt="my lovely sister"/><image src="https://github.com/ShieldQiQi/FourAxisManipulator/blob/masterX/Picture/%E5%B0%8F%E6%B7%B7%E6%B2%8C02.png" width="180" alt="my lovely sister"/><image src="https://github.com/ShieldQiQi/FourAxisManipulator/blob/masterX/Picture/%E5%B0%8F%E6%B7%B7%E6%B2%8C04.png" width="180" alt="my lovely sister"/>
***


## 2019/12/08

Finish transport a Chinese string from `Qt widget` to `Ros` stream then to `Freetype` demo

Solve the problem that the newest text carry the before one.
<br>
According to the offical doucuments<br>
```
  /**************************************************************************
   *
   * @function:
   *   FT_New_Face
   *
   * @description:
   *   Call @FT_Open_Face to open a font by its pathname.
   *
   * @inout:
   *   library ::
   *     A handle to the library resource.
   *
   * @input:
   *   pathname ::
   *     A path to the font file.
   *
   *   face_index ::
   *     See @FT_Open_Face for a detailed description of this parameter.
   *
   * @output:
   *   aface ::
   *     A handle to a new face object.  If `face_index` is greater than or
   *     equal to zero, it must be non-`NULL`.
   *
   * @return:
   *   FreeType error code.  0~means success.
   *
   * @note:
   *   Use @FT_Done_Face to destroy the created @FT_Face object (along with
   *   its slot and sizes).
   */
  FT_EXPORT( FT_Error )
  FT_New_Face( FT_Library   library,
               const char*  filepathname,
               FT_Long      face_index,
               FT_Face     *aface );
```
<br>So when in the case you frequently use the function, destory the face use "FT_Done_Face(face)" and make sure the image buffer is cleared to all zero.


## 2019/12/11

add RVIZ visualization of the Text

## 2019/12/13

Try to Simplify the Text to dashs from outlines first before judge what the strok is.

## 2019/12/16

Simplify the Text to dashs from outlines and solid first, then sort the pointqueue according to 
<br>
hand-writing habits. But the effects is not obvious, need for more revolution...

## 2019/12/18

add inverse dynamic solution from pointQueue, send the joint space angles to stm32...
<br>
approve the algorithm of sorting the pointqueue...

## 2019/12/23

solve bug that points number increase after sort the points order..
<br>
solve bug that text occur distortion when write in reality..
<br>
distortion seems to be vanisheded after change the X and Y bias of the whole text..

## 2019/12/24

improve code to write at most five texts in one time..

## 2019/12/27


correct the sorted path to make it more continuous and add "Anti-aliased" function...
<br>
use handwriting-zh_CN-gb2312.xml to get the right strokes order and make every stroke a whole points queue no matter how  complicated it is..

