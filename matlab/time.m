%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                         %
%                            Process the times                            %
%                                                                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;
%clc;

filename = '../results/test_ss_parallel_time.dat';
parallel = true;

delimiterIn = ',';
headerlinesIn = 1;
data = importdata(filename,delimiterIn,headerlinesIn);
data = data.data;

if parallel == true
   total_time = data(numel(data));
   
   k=1;
   for i=3:2:numel(data)-5
       iteration_time(k) = data(i);
       sending_time(k) = data(i+1);
       k = k+1;
   end
   
   display(['Total time [s]: ', num2str(total_time)]);
   display(['Mean iteration time [s]: ', num2str(mean(iteration_time))]);
   display(['Max iteration time [s]: ', num2str(max(iteration_time))]);
   display(['Min iteration time [s]: ', num2str(min(iteration_time))]);
   display(['Mean sending time [s]: ', num2str(mean(sending_time))]);
   display(['Max sending time [s]: ', num2str(max(sending_time))]);
   display(['Min sending time [s]: ', num2str(min(sending_time))]);   
    
else
    total_time = data(numel(data));
    
   k=1;
   for i=3:1:numel(data)-5
       iteration_time(k) = data(i);
       k = k+1;
   end
   
   display(['Total time [s]: ', num2str(total_time)]);
   display(['Mean iteration time [s]: ', num2str(mean(iteration_time))]);
   display(['Max iteration time [s]: ', num2str(max(iteration_time))]);
   display(['Min iteration time [s]: ', num2str(min(iteration_time))]);    
    
    
end