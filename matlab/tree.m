clear all;
close all;
clc;

filename = './test.dat';

delimiterIn = ',';
headerlinesIn = 0;
data = importdata(filename,delimiterIn,headerlinesIn);

figure()
hold all;

for i=0:length(data)/4-1
    x = data(4*i+1);
    y = data(4*i+2);
    w = data(4*i+3);
    h = data(4*i+4);
    plot(x,y, 'k*');
    line([x-w/2 x+w/2], [y+h/2 y+h/2], 'Color', 'b');
    line([x-w/2 x+w/2], [y-h/2 y-h/2], 'Color', 'b');
    line([x+w/2 x+w/2], [y-h/2 y+h/2], 'Color', 'b');
    line([x-w/2 x-w/2], [y-h/2 y+h/2], 'Color', 'b');
end

plot(33, 14.4, '*r');
plot(35, 14, '*r');
plot(-30, 30, '*r');
plot(-10, 10, '*r');
