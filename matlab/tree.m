clear all;
close all;
clc;

filename = '../results/bh_1e2_quadtree.dat';

delimiterIn = ',';
headerlinesIn = 0;
%data = importdata(filename,delimiterIn,headerlinesIn);

size = 100;
percentage = 0;

disp('Loading the data');

fid = fopen(filename);
textLine = fgets(fid); % Read first line.
lineCounter = 1;
while ischar(textLine)
    %fprintf('\nLine #%d of text = %s\n', lineCounter, textLine);
	% get into numbers array.
	numbers = sscanf(textLine, '%f, ');
	% Put numbers into a cell array IF and only if
	% you need them after the loop has exited.
	% First method - each number in one cell.
	for k = 1 : length(numbers)
		data{lineCounter, k} = numbers(k);
	end
	% ALternate way where the whole array is in one cell.
	data2{lineCounter} = numbers;
	
	% Read the next line.
    textLine = fgets(fid);
	lineCounter = lineCounter + 1;
end
fclose(fid);

disp('Finished loading the data');

f = figure();

for j=1:length(data(:,1))
    tic
    
    subplot(1,1,1,'replace');
    hold all;
    
    length(data(j,:))
    
    i = 0;
    while true
        if 4*i+2 > length(data(j,:))
            break;
        end
        x = data(j,4*i+2);
        y = data(j,4*i+3);
        w = data(j,4*i+4);
        h = data(j,4*i+5);
        x = cell2mat(x);
        y = cell2mat(y);
        w = cell2mat(w);
        h = cell2mat(h);
        
        if isempty(x)
            break;
        end
        if w>percentage*size/100 && h>percentage*size/100
            %plot(x,y, 'k*');
            line([x-w/2 x+w/2], [y+h/2 y+h/2], 'Color', 'b');
            line([x-w/2 x+w/2], [y-h/2 y-h/2], 'Color', 'b');
            line([x+w/2 x+w/2], [y-h/2 y+h/2], 'Color', 'b');
            line([x-w/2 x-w/2], [y-h/2 y+h/2], 'Color', 'b');
            %drawnow;
        end
        i = i+1;
    end
    drawnow;
    toc
    break;
end

% Add the positions
filename = '../results/bh_1e2.dat';
delimiterIn = ',';
headerlinesIn = 1;
data = importdata(filename,delimiterIn,headerlinesIn);
data = data.data;
idx = 1;
length = length(data);
iterations = 0;
id = [];
mass = [];
x = [];
y = [];
time = data(idx, 1);
while data(idx, 1) == time && idx < length
   mass = [mass;  data(idx, 2)];
   x = [x;  data(idx, 3)];
   y = [y;  data(idx, 4)];
   idx = idx +1;  
end
scatter(x,y, 10, 'k', 'filled');

xlabel('x [AU]');
ylabel('y [AU]');
