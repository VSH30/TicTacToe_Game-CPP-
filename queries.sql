CREATE DATABASE cppgame;
USE cppgame;
CREATE TABLE xox(gameid INT(3) AUTO_INCREMENT PRIMARY KEY, player_A VARCHAR(30), player_B VARCHAR(30), status VARCHAR(30), moves INT(9));
INSERT INTO xox(player_A, player_B, status, moves) VALUES('VSH','XYZ','VSH',7654321);
SELECT * FROM xox;
DROP DATABASE cppgame;