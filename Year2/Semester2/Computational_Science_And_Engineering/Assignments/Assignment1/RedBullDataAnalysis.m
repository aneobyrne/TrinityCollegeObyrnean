
%Red Bull Data plotted.
close all
clear 
% Read data from file
jumpdata = csvread('RedBullJumpData.csv');
t_redbull = jumpdata(:,1); %s a vector containing the timestamps at which the velocities in v_redbull were measured.
v_redbull = jumpdata(:,2); %s a vector containing the velovities
terminal_velocity = jumpdata(:,3); % Third column is theoretical terminal velocity at that time (You need to use this in the last question)
N_timestamps = length(t_redbull); %The number of instants at which the time and velocity pairs were measured
%Calculate freefall velocity vector here
g = 9.81;
v_freefall = g*t_redbull; 
% Part 1
% We are giving you this answer for free
figure(1);
h_part1 = plot(t_redbull, v_redbull, 'r-x', 'linewidth', 2.0);
shg;
grid on;
%X-AXIS
xlabel('Time (secs)', 'fontsize', 24, 'Color','blue');
xlim([-inf 180]);
%Y-AXIS
ylabel('Velocity (m/s)', 'fontsize', 24, 'Color','blue');
ylim([-inf 400]);
hold on;
% Part 2
% This plot does not have the right linewidth. You fix it.
h_part2 = plot(t_redbull, v_freefall, 'k--', 'linewidth', 2.0); 
shg;
% This is how to put on a grid 
grid on;
% This is how to fix an axis to a desired size
% axis([something goes in here]);
xlim([-inf 180]);
ylim([-inf 400]);
% Set the fontsize and label the graph here!!
xlabel('Time (secs)', 'fontsize', 24,'Color','blue');
ylabel('Velocity (m/s)', 'fontsize', 24,'Color','blue');
% Calculate when he hits the atmosphere
% Part 3
% Need some stuff here ... or read it off from the graph
deviation = v_freefall - v_redbull;
dev_percentage = deviation./v_freefall;
dev_index = find(dev_percentage >= 0.02, 1);
hit_instant = t_redbull(dev_index);
fprintf('Mr. B hits the earth''s atmoshpere at %f secs after he jumps \n', hit_instant);
% Part 4
% Now starting from the velocity and time = 56 secs 
% .. let's update and calculate v
g = 9.81;
v_numerical_1 = zeros(size(t_redbull));
drag_constant = 3/60;
start = find(t_redbull == 56);
final = find(t_redbull == 180);
v_numerical_1(start) = v_redbull(start);
% Starting from this time instant, calculate the velocity required
for k = start + 1 : final
 
v_numerical_1(k) = v_numerical_1(k-1) + (g - (drag_constant*v_numerical_1(k-1)))*(t_redbull(k) - (t_redbull(k - 1)));
end
t_euler = t_redbull(start:final);
v_numerical = v_numerical_1(start:final);
% Plot using the dashed green line with (+) markers
h_part4 = plot(t_euler, v_numerical, 'go--','linewidth',2.0, 'markersize', 2.5);shg
% Part 5 
% Calculate the percentage error as required
index_t1= find(t_redbull == 69);
index_t2= find(t_redbull == 180);
per_error(1) = abs((v_numerical_1(index_t1) - v_redbull(index_t1))/v_redbull(index_t1))*100; % This is just some random number .. which is 
wrong
per_error(2) = abs((v_numerical_1(index_t2) - v_redbull(index_t2))/v_redbull(index_t2))*100;
fprintf('The percentage error at 69 and 180 secs is %1.1f and\n', per_error(1));
fprintf('%3.1f respectively \n', per_error(2));
% Part 6 
% You'll need to repeat your euler loop here again but this time

% update the drag constant at every timestamp and change the update
% calculation to allow for the new v^2(t) term
% A hint here that now you have to calculate the velocity using the new
% differental equation
% constant .. put it in v_numerical_2
v_numerical_2 = zeros(N_timestamps,1);
start = find(t_redbull == 56);
final = find(t_redbull == 100);
v_numerical_2(start) = v_redbull(start);
% Starting from this time instant, calculate the velocity required
for k = start : N_timestamps - 2
 
new_drag_constant = g/(terminal_velocity(k+1)^2);
 
h = (t_redbull(k+1) - t_redbull(k));
 
%predictor:
 
euler_dvdt = (g - (new_drag_constant*(v_numerical_2(k))^2));
 
v_euler = v_numerical_2(k) + euler_dvdt*h;
 
%corrector:
 
new_drag_constant2 = g/(terminal_velocity(k+2)^2);
 
heuns_dvdt = (g - (new_drag_constant2*(v_euler)^2));
 
v_numerical_2(k+1) = v_numerical_2(k) + (h/2)*(euler_dvdt + heuns_dvdt);
end
% This is the handle plot for part 6. You have to plot the right stuff not
% this stuff.
% Note that the plot linewidth and colour are wrong. Fix it.
h_part6 = plot(t_redbull(start:final), v_numerical_2(start:final), 'k-+','linewidth',2.0);
shg
index_t3= find(t_redbull == 100);
est_error = (abs(v_numerical_2(index_t3) - v_redbull(index_t3))/v_redbull(index_t3))*100;
fprintf('The error at t = 100 secs using my estimated drag information is %f\n', est_error);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% DO NOT EDIT BELOW THIS LINE THIS IS TO MAKE SURE YOU HAVE USED THE
% VARIABLES THAT WE ASKED FOR
% Check for existence of variables
if (~exist('v_freefall', 'var'))
 error('The variable v_freefall does not exist.')
end;
if (~exist('hit_instant', 'var'))
 error('The variable hit_instant does not exist.')
end;
if (~exist('per_error', 'var'))
 error('The variable per_error does not exist.')
end;
if (exist('per_error', 'var'))
 l = size(per_error);
 if ( sum(l - [1 2]) ~= 0)
 
error('per_error is not a 2 element vector. Please make it so.')
 end;
end;
if (~exist('v_numerical_1', 'var'))
 error('The variable v_numerical_1 does not exist.')
end; 
if (~exist('est_error', 'var'))
 error('The variable est_error does not exist.')
end; 
if (~exist('h_part1', 'var'))
 error('The plot handle h_part11 is missing. Please create it as instructed.')
end;
if (~exist('h_part2', 'var'))
 error('The plot handle h_part11 is missing. Please create it as instructed.')
end;
if (~exist('h_part4', 'var'))
 error('The plot handle h_part11 is missing. Please create it as instructed.')
end;
if (~exist('h_part6', 'var'))
 error('The plot handle h_part11 is missing. Please create it as instructed.')
end;
