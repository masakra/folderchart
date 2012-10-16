
PRAGMA foreign_keys = ON;

DROP TABLE IF EXISTS folders;

CREATE TABLE folders (
	id				INTEGER PRIMARY KEY,
	parent_id		INTEGER NOT NULL,
	name			TEXT NOT NULL,
	path			TEXT NOT NULL,
	size			INTEGER NOT NULL
);

DROP TABLE IF EXISTS files;

CREATE TABLE files (
	id				INTEGER PRIMARY KEY,
	folders_id		INTEGER NOT NULL,
	name			TEXT NOT NULL,
	size			INTEGER NOT NULL,
	FOREIGN KEY ( folders_id ) REFERENCES folders ( id )
);

DROP TABLE IF EXISTS percents;

CREATE TABLE percents (
	id				INTEGER PRIMARY KEY,
	folders_id		INTEGER NOT NULL,
	type			TEXT NOT NULL,
	quantity		INTEGER NOT NULL,
	percent			REAL,
	FOREIGN KEY ( folders_id ) REFERENCES folders ( id )
);

