# FourAxisManipulator

## 2019/12/08

Finish transport a Chinese string from `Qt widget` to `Ros` stream then to `Freetype` demo

Solve the problem that the newest text carry the before one.
<br>
According to the offical doucuments<br>
![image] https://github.com/ShieldQiQi/FourAxisManipulator/blob/master/Picture/FreetypeDocument.png
<br>So when in the case you frequently use the function, destory the face use "FT_Done_Face(face)" and make sure the image buffer is cleared to all zero.


## 2019/12/11

add RVIZ visualization of the Text

## 2019/12/13

Try to Simplify the Text to dashs from outlines first before judge what the strok is.

## 2019/12/16

Simplify the Text to dashs from outlines and solid first, then sort the pointqueue according to 
<br>
hand-writing habits. But the effects is not obvious, need for more revolution...

