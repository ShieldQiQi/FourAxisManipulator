# FourAxisManipulator

## 2019/12/08
```
Finish transport a Chinese string from `Qt widget` to `Ros` stream then to `Freetype` demo

Solve the problem that the newest text carry the before one.
<br>
According to the offical doucuments<br>
![image] (https://github.com/ShieldQiQi/FourAxisManipulator/blob/master/Picture/FreetypeDocument.png)
<br>So when in the case you frequently use the function, destory the face use "FT_Done_Face(face)" and make sure the image buffer is cleared to all zero.
```

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

```
correct the sorted path to make it more continuous and add "Anti-aliased" function...

use handwriting-zh_CN-gb2312.xml to get the right strokes order and make every stroke a whole points queue no matter how  complicated it is..
```
