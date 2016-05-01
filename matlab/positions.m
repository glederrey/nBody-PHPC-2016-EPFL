%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                         %
%              Plot the different objects in a single figure              %
%                                                                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clc;
clear all;
close all;

filename = '../results/test_ss_parallel.dat';
solar_system = true;
movie = true;
size = 5;

delimiterIn = ',';
headerlinesIn = 1;

if solar_system
    sun = [247, 115, 12]./255;
    mercury = [250, 248, 242]./255;
    venus = [255, 255, 242]./255;
    earth = [11, 92, 227]./255;
    mars = [240, 198, 29]./255;
    jupiter = [253, 199, 145]./255;
    saturn = [224, 196, 34]./255;
    uranus = [220, 241, 245]./255;
    neptune = [57, 182, 247]./255;
end

gray = [100, 100, 100]./255;

data = importdata(filename,delimiterIn,headerlinesIn);
data = data.data;

scrsz = get(0,'ScreenSize');
f = figure('units','pixels','Position',[1 scrsz(4) scrsz(3) scrsz(4)]);
hold all;
idx = 1;
length = length(data);
iterations = 0;

if movie == true
    mov = VideoWriter('Gravitation.avi','Motion JPEG AVI');
    open(mov);
end

while iterations < 100
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
    
    if time == 0
        nbr_bodies_start = numel(mass);
    end
    
    if idx >= length
        nbr_bodies_end = numel(mass);
    end
    
    subplot(1,1,1,'replace');
    hold all;
    
    if solar_system
       scatter(x(10:end),y(10:end), 3, gray, 'filled'); 
       
       plot(x(1),y(1),'Color', sun, 'Marker','.','Markersize',40);
       plot(x(2),y(2),'Color', mercury, 'Marker','.','Markersize',20);
       plot(x(3),y(3),'Color', venus, 'Marker','.','Markersize',20);
       plot(x(4),y(4),'Color', earth, 'Marker','.','Markersize',20);
       plot(x(5),y(5),'Color', mars, 'Marker','.','Markersize',20);
       plot(x(6),y(6),'Color', jupiter, 'Marker','.','Markersize',10);
       plot(x(7),y(7),'Color', saturn, 'Marker','.','Markersize',10);
       plot(x(8),y(8),'Color', uranus, 'Marker','.','Markersize',10);
       plot(x(9),y(9),'Color', neptune, 'Marker','.','Markersize',10);       
        
    end
    
    set(gcf, 'color', [0 0 0])
    set(gca, 'color', [0 0 0])
    set(gca,'visible','off');
    axis([-size size -size size]);
    
    axis('square');
    
    drawnow;
    
    iterations = iterations + 1;
    
    if movie == true
        F = getframe(gcf);
        writeVideo(mov,F);
        writeVideo(mov,F);
    end
end

close(f);
if movie == true
    close(mov);
end

disp(['Number of bodies at the beginning of the simulation: ', num2str(nbr_bodies_start)]);
disp(['Number of bodies at the end of the simulation: ', num2str(nbr_bodies_end)]);
disp(['Percentage of bodies lost: ', num2str((nbr_bodies_start-nbr_bodies_end)/nbr_bodies_start*100), '%']);