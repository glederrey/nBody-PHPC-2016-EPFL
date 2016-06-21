%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                         %
%              Plot the different objects in a single figure              %
%                                                                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clc;
clear all;
close all;

files = '../results2/ss_1e6';
startTime=0;
endTime=1999;
solarSystem = true;
movie = true;
movieName = 'SS_1e6_5AU.avi';
fullScreen = true;
center = 'sun';
size = 5;

if solarSystem
    sun = [198, 81, 4]./255;
    mercury = [200, 208, 202]./255;
    venus = [164, 92, 23]./255;
    earth = [11, 92, 227]./255;
    mars = [172, 138, 97]./255;
    jupiter = [171, 165, 165]./255;
    saturn = [176, 150, 110]./255;
    uranus = [169, 205, 210]./255;
    neptune = [56, 83, 202]./255;
end

gray = [100, 100, 100]./255;

if fullScreen == true
    scrsz = get(0,'ScreenSize');
    f = figure('units','pixels','Position',[1 scrsz(4) scrsz(3) scrsz(4)]);
else
    f = figure();
end
hold all;

if movie == true
    mov = VideoWriter(movieName,'Motion JPEG AVI');
    open(mov);
end

nbrBodiesStart = 0;
nbrBodiesEnd = 0;

for iter=startTime:endTime
    
    display(['Iteration ', num2str(iter)]); 
    
    filename = [files, '_', num2str(iter), '.dat'];
    delimiterIn = ',';
    if iter==startTime
        data = importdata(filename,delimiterIn,1);
        data = data.data;
    else
       data = importdata(filename,delimiterIn, 0);
    end
    
    nbrBodies = length(data);
    
    if iter==startTime
        nbrBodiesStart=nbrBodies;
    end
    
    if iter==endTime
        nbrBodiesEnd=nbrBodies;
    end
    
    subplot(1,1,1,'replace');
    hold all;

    if solarSystem
       scatter(data(10:end,3),data(10:end,4), 1, gray, 'filled');
       
       % Uncomment this to check the distance Eart-Sun (should be around 1)
       %sqrt((x(4)-x(1))^2 + (y(4)-y(1))^2)
       
       plot(data(1,3),data(1,4),'Color', sun, 'Marker','.','Markersize',40);
       plot(data(2,3),data(2,4),'Color', mercury, 'Marker','.','Markersize',20);
       plot(data(3,3),data(3,4),'Color', venus, 'Marker','.','Markersize',20);
       if strcmp(center,'earth')
           plot(data(4,3),data(4,4),'Color', earth, 'Marker','.','Markersize',40);
       else
           plot(data(4,3),data(4,4),'Color', earth, 'Marker','.','Markersize',20);
       end
       plot(data(5,3),data(5,4),'Color', mars, 'Marker','.','Markersize',20);
       plot(data(6,3),data(6,4),'Color', jupiter, 'Marker','.','Markersize',20);
       plot(data(7,3),data(7,4),'Color', saturn, 'Marker','.','Markersize',20);
       plot(data(8,3),data(8,4),'Color', uranus, 'Marker','.','Markersize',20);
       plot(data(9,3),data(9,4),'Color', neptune, 'Marker','.','Markersize',20);       
        
    end
    
    set(gcf, 'color', [0 0 0])
    set(gca, 'color', [0 0 0])
    set(gca,'visible','off');
    if strcmp(center,'earth')
        axis([data(4,3)-size data(4,3)+size data(4,4)-size data(4,4)+size]);
    else
        axis([-size size -size size]);
    end
        
    
    axis('square');
    
    drawnow;
    
    if movie == true
        F = getframe(gcf);
        writeVideo(mov,F);
        writeVideo(mov,F);
    end
end

%close(f);
if movie == true
    close(mov);
end

disp(['Number of bodies at the beginning of the simulation: ', num2str(nbrBodiesStart)]);
disp(['Number of bodies at the end of the simulation: ', num2str(nbrBodiesEnd)]);
disp(['Percentage of bodies lost: ', num2str((nbrBodiesStart-nbrBodiesEnd)/nbrBodiesStart*100), '%']);