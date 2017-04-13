-- the following script is utilized by the mmns server
-- to initialize a database that does not yet exist.

-- CREATE the node stats
CREATE TABLE node_stats (
	TBL_INDEX INTEGER UNIQUE NOT NULL,
	IP_ADDR   VARCHAR(45) NOT NULL, -- we include ipv6 in this
	PORT      INTEGER NOT NULL,
	HOSTNAME  VARCHAR(128) NOT NULL,

	TIMESTAMP INTEGER NOT NULL
);

-- CREATE the USERS stats
CREATE TABLE users_stats (
	TBL_INDEX INTEGER UNIQUE NOT NULL,
	IP_ADDR   VARCHAR(45) NOT NULL, -- ipv6

	USERNAME  VARCHAR(64) NOT NULL,
	PASSWORD  VARCHAR(64) NOT NULL,

	TIMESTAMP INTEGER NOT NULL
);
