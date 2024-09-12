format long
N = 8; % The number of points.
ts = (0:-1:-N + 1)';
A = [ts.^2 ts.^1 ts.^0]; % Raise each entry of 'ts' to 2, 1 and 0
B = inv( A'*A )*A'
format long
N = 8; % The number of points.
ts = (0:-1:-N + 1)';
A = [ts.^2 ts.^1 ts.^0]; % Raise each entry of 'ts' to 2, 1 and 0.
detAtA = det( A'*A ) % This should be an integer.
detAtA = round( detAtA ) % Round it to the nearest integer.
C = detAtA * inv( A'*A )*A' % This should be an integer array.
C = round( C ) % Round each entry to the nearest integer.
C = C / detAtA
B - C % If you did calculate 'C', compare the two.