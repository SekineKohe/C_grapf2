set terminal png size 1024,768
set output 'plot.png'

set multiplot layout 4,1
set lmargin at screen 0.15
plot 'hoge.csv' using 1:2 with lines
plot 'hoge.csv' using 1:3 with lines
plot 'hoge.csv' using 1:4 with lines
plot 'hoge.csv' using 1:5 with lines
     
