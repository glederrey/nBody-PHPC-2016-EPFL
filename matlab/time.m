%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                         %
%                            Process the times                            %
%                                                                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;
%clc;

filename = '../results/bf_1e6_time.dat';
parallel = false;
%algo = 'barnes-hut';
algo = 'brute-force';

delimiterIn = ',';
headerlinesIn = 1;
data = importdata(filename,delimiterIn,headerlinesIn);
data = data.data;

if parallel == false
    if strcmp(algo,'brute-force')
        total_time = data(numel(data));
        sim_time = data(numel(data)-1);
        loading_time = data(1);
        
        k=1;
        for i=2:1:numel(data)-2
            iteration_time(k) = data(i);
            k = k+1;
        end        
        
        display(['Total time: ', num2str(total_time), ' [s]']);
        display(['Loading time: ', num2str(loading_time), ' [s]']);
        display(['Mean iteration time: ', num2str(mean(iteration_time)), ' [s]']);
        display(['Max iteration time: ', num2str(max(iteration_time)), ' [s]']);
        display(['Min iteration time: ', num2str(min(iteration_time)), ' [s]']);
        display(['Proportion of serial time on 100 iterations: ', num2str(loading_time/(100*mean(iteration_time)))]);
        display(['Proportion of serial time on 1000 iterations: ', num2str(loading_time/(1000*mean(iteration_time)))]);
    elseif strcmp(algo,'barnes-hut')
        total_time = data(numel(data));
        sim_time = data(numel(data)-1);
        loading_time = data(1);
        building_time(1) = data(2);
        
        k=1;
        for i=3:2:numel(data)-5
            iteration_time(k) = data(i);
            building_time(k+1) = data(i+1);
            k = k+1;
        end        
        
        display(['Total time: ', num2str(total_time), ' [s]']);
        display(['Loading time: ', num2str(loading_time), ' [s]']);
        display(['Mean iteration time: ', num2str(mean(iteration_time)), ' [s]']);
        display(['Max iteration time: ', num2str(max(iteration_time)), ' [s]']);
        display(['Min iteration time: ', num2str(min(iteration_time)), ' [s]']);
        display(['Mean building time: ', num2str(mean(building_time)), ' [s]']);
        display(['Max building time: ', num2str(max(building_time)), ' [s]']);
        display(['Min building time: ', num2str(min(building_time)), ' [s]']);
        display(['Proportion of building in iteration: ', num2str(100*mean(building_time)/mean(iteration_time)), ' %']);
        display(['Proportion of serial time on 100 iterations: ', num2str(loading_time/(100*mean(iteration_time)))]);
        display(['Proportion of serial time on 1000 iterations: ', num2str(loading_time/(1000*mean(iteration_time)))]);
    end
else
    if strcmp(algo,'brute-force')
        total_time = data(numel(data));
    elseif strcmp(algo,'barnes-hut')
        total_time = data(numel(data));
    end
end
