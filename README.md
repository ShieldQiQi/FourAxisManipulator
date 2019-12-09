# FourAxisManipulator

## 2019/12/08

### Finish transport a Chinese string from `Qt widget` to `Ros` stream then to `Freetype` demo

### Solve the problem that the newest text carry the before one.
<br>
According to the offical doucuments:
<br>
![Image text](https://github.com/ShieldQiQi/FourAxisManipulator/blob/master/Picture/FreetypeDocument.png)
<br>
So when in the case you frequently use the function, destory the face use "FT_Done_Face(face)" and make sure the image buffer is cleared to all zero.
