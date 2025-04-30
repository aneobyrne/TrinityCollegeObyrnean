close all
clear % Please remember to uncomment this when you submit your code as an 
% assignment submission.
% Read in the picture
original = double(imread('greece.tif')); % Original uncorrupted image
mask = double(imread('badpixels.tif')); % Read in an indicator picture 
% which is 1 where the pixels are missing in badicture
% Load required data
%load greece.tif; % Original uncorrupted image
load badpicture.mat; % Corrupted image which contains holes named "badpic"
%load badpixels.mat; % Mask indicating corrupted pixels
load forcing.mat; % Forcing function f
% Set parameters
total_iterations = 2500; % Total iterations
alpha = 0.99; % constant (it is said 0.8, but 0.99 leads to a better result)
% Initialize restored images after max iterations
N = badpic;
restored = N; % Without forcing function
restored2 = N; % With forcing function
restored20 = [];
restored20_2 = [];
% Initialize error vectors: arrays of zeros with total_iterations rows 
% and 1 column
err = zeros(1, total_iterations);
err2 = zeros(1, total_iterations);
abs_2 = zeros(1, total_iterations);
% This displays the original picture in Figure 1
figure(1);
image(original);
title('Original');
colormap(gray(256));
% Display the corrupted picture in Figure 2
figure(2);
image(badpic);
title('Corrupted');
colormap(gray(256));
%[m, n] = size(badpic);
[j, i] = find(mask ~= 0);
%[m, n] = find(mask == 0); % This stores all the locations in vectors i 
% and j (row and column indices for each missing pixel)
% You might want to also store those locations in a different format
% You will need two images to store your iterations as you go forward
% Because you will be writing your next iteration into another image based 
% on the current image
% Suppose "restored" is the current estimate of your solution at iteration k
% then you'll be processing "restored" and writing the output into 
% "restored_tmp" for example as you visit each pixel
% So after each iteration, the next k+1th result is in "restored_tmp"
% which means after the kth iteration, you have to copy restored_tmp into 
% restored to initialise the next iteration.
% This is synchronous updating. It turns out that this kind of updating 
% isn't that good in this case but we are using it anyway.
for iteration = 1 : total_iterations 
 % Update restored image without forcing function
 I = restored; 
 
for k = 1 : length(i)
 
m = j(k);
 
n = i(k); 
 
% Calculate updated value using SOR scheme
 
E1 = (restored(m - 1, n) + restored(m + 1, n) + restored(m, n - 1)); 
 
E = E1 + (restored(m, n + 1) - 4 * restored(m, n));
 
I(m, n) = restored(m, n) + (alpha * E) / 4;
 
abs_2(k) = abs (original(j(k), i(k)) - restored(j(k), i(k)));
 
end
 % you have to iterate over all the missing pixels (don't iterate over 
 % the whole image!) only
 
% and at each location you update your restored image using the 2D FDM 
 
% equation in the assignment description
 
% Remember to index a pixel value in a picture "pic" at row 3 and 
 
% column 4 its pic(3, 4) 
 % And when you get to the 20th iteration, you store your restored image 
 % in "restored20" 
 % And after each iteration you calculate the std devation between the 
 % original and restored images
 % but only in the location of the missing pixels!

 
restored = I; % Without forcing function
 % Calculate error
 %err(iteration) = sum(abs(original(mask == 1) - restored(mask == 1))) / 
 % sum(mask(:));
 
err(iteration) = mean(abs_2);
 % Rrestored image after 20 iterations
 
if iteration == 20
 
restored20 = restored;
 
end
end
% Display the restored image in Figure 3 (This does NOT use the forcing 
% function)
figure(3);
image(restored);
title('Restored Picture (Without Forcing)');
colormap(gray(256));
% Now repeat the restoration, again starting from the badpicture, 
% but now use the forcing function in your update
% Remember the thing above about restored and restored_tmp
for iteration = 1 : total_iterations 
 % Update restored image without forcing function
 I2 = restored2; 
 
for k = 1 : length(i)
 
m = j(k);
 
n = i(k); 
 
% Calculate updated value using SOR scheme
 
E1 = (restored2(m - 1, n) + restored2(m + 1, n) + restored2(m, n - 1));
 
E = E1 + (restored2(m, n + 1) - 4 * restored2(m, n) - f(m, n)); 
 
I2(m, n) = restored2(m, n) + (alpha * E) / 4;
 
abs_2(k) = abs (original(j(k), i(k)) - restored2(j(k), i(k)));
 
end
 
restored2 = I2; % With forcing function
 
% Calculation of Mean Absolute Error for each iteration
 
%err2(iteration) = sum(abs(double(original(mask == 1)) - 
 
% double(restored2(mask == 1)))) / sum(mask(:));
 
err2(iteration) = mean(abs_2);
 
% Restored images after 20 iterations
 
if iteration == 20
 
restored20_2 = restored2;
 
end
end
% Display your restored image with forcing function as Figure 4
figure(4);
image(restored2);
title('Restored Picture (With Forcing)');
colormap(gray(256));
% And plot your two error vectors versus iteration
figure(5);
iterations = 1:total_iterations;
plot(iterations, err, 'r-', iterations, err2, 'b-', 'linewidth', 3);
legend('No forcing function', 'With forcing function');
xlabel('Iteration', 'fontsize', 20);
ylabel('Std Error', 'fontsize', 20);
