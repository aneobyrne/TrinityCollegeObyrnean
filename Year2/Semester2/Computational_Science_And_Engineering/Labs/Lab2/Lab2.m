
% close all
% clear all
% You normally need these two lines, but in Matlab grader they break the reference solution

% Setting parameters
m = 70;    % mass of parachutist [kg]
g = 9.81;  % gravitational constant [m/s]
c = 15.5;  % drag coefficient [Ns/m]

% Create the vector (array, list) of time values 
delta_t = 0.5; % you have to change this
t = 4: delta_t : 24; % set your time vector here with a step of delta_t

% Task 1: Calculate the analytic velocity at each time stamp
analytic_velocity = ((m*g)/c)*(1 - exp(- (c*t)/m)); 

% The analytic results are plotted here
figure(1);
plot(t, analytic_velocity, 'r-*', 'linewidth', 3);
xlabel('Time (secs)', 'fontsize', 20);
ylabel('Velocity (m/s)', 'fontsize', 20);

% Task 2: Now use Euler's solution to do the same thing numerically
% First set the time step (h) for the solution
h = 2.0;
% and now the time vector for plotting the euler's solution
t_euler = (4 : h : 24); % you have to change this
% Then set up N (the number of time values at which you wish to calculate the solution)
N = length(t_euler);
% Set a constant for convenience
drag_const = c / m; 
% Now initialise the numerical solution itself
numerical_solution = zeros(size(t_euler));
% Set up the initial condition in the solution
% At t = 4, the velocity is assumed to be the same as the analytic_velocity at that time
numerical_solution(1) = analytic_velocity(1);  %hence change this to the right intial velocity

% Now calculate the numerical_solution at each timestamp k
% You can use either a for-loop or a while-loop but a for-loop makes more sense here
% Insert your code here
for k = 2:N
    numerical_solution(k) = numerical_solution(k-1) + (g - (drag_const*numerical_solution(k-1)))*h;
end

% Task 3: Now plot the two solutions on the same plot
figure(2);
handle = plot(t, analytic_velocity, 'g-+', t_euler, numerical_solution, 'r-o');
grid on;
% Now sort out linewidths and label your axes here .. you'll have to change this
set(handle,'linewidth', 3.0);
xlabel('Time (secs)', 'fontsize', 20);
ylabel('Velocity (m/s)', 'fontsize', 20);
legend('Analytic', 'Numerical');
% And don't forget the legend! Use ">>help legend" in Matlab to see how to use it.

% Task 4: Calculate the mean abs difference between t = 4 and 14 secs inclusive
% you need to calculate the differences at the timestamps in t_euler between that interval
% as an example, if you want to calculate the mean absolute error between two vectors v1 and v2 of the same length
% e = abs(v1 - v2) gives you the error and mean(abs(e)) generates the mean abs error.
% Insert your calculation here
absolute_value = abs(analytic_velocity(4 : 14) - numerical_solution(4 : 14));
modelling_error = mean(absolute_value); % you have to change this 
fprintf(' The MAE over t = 4:24 secs is %f\n', modelling_error);
