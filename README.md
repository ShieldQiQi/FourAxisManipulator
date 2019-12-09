# FourAxisManipulator

## 2019/12/08

### Finish transport a Chinese string from `Qt widget` to `Ros` stream then to `Freetype` demo

### Solve the problem that the newest text carry the before one.
<br>
According to the offical doucuments
```
git clone https://github.com/AIDungeon/AIDungeon/
cd AIDungeon
./install.sh
python play.py
```
So when in the case you frequently use the function, destory the face use "FT_Done_Face(face)" and make sure the image buffer is cleared to all zero.
