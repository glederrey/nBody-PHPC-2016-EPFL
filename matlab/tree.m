clear all;
close all;
clc;

filename = '../results/test_ss_quadtree.dat';

delimiterIn = ',';
headerlinesIn = 0;
data = importdata(filename,delimiterIn,headerlinesIn);

size = 100;
percentage = 0;

figure()
hold all;

length(data)

tic

for i=0:(length(data)-1)/4-1
    x = data(4*i+2);
    y = data(4*i+3);
    w = data(4*i+4);
    h = data(4*i+5);
    if w>percentage*size/100 && h>percentage*size/100
        %plot(x,y, 'k*');
        line([x-w/2 x+w/2], [y+h/2 y+h/2], 'Color', 'b');
        line([x-w/2 x+w/2], [y-h/2 y-h/2], 'Color', 'b');
        line([x+w/2 x+w/2], [y-h/2 y+h/2], 'Color', 'b');
        line([x-w/2 x-w/2], [y-h/2 y+h/2], 'Color', 'b');
        %drawnow;
    end
end

toc

%plot(33, 14.4, '*r');
%plot(35, 14, '*r');
%plot(-30, 30, '*r');
%plot(-10, 10, '*r');
