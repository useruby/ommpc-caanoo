#include "songDb.h"
#include <stdexcept>

using namespace std;

vector<pair<string, string> > resultSet;
vector<string> singleResult;
static int callback(void *notUsed, int numCols, char** dbResults, char** colName) {
	int i;
	string file;
	string item;
	for(i=0; i<numCols; i++){
		if(strcmp(colName[i], "path") == 0 ) {
			file.assign(dbResults[i]);
		} else {
			item.assign(dbResults[i]);
		}
	}
	resultSet.push_back(make_pair(item, file));
	return 0;
}

static int callback2(void *notUsed, int numCols, char** dbResults, char** colName) {
	int i;
	string item;
	for(i=0; i<numCols; i++){
		item.assign(dbResults[i]);
	}
	singleResult.push_back(item);
	return 0;
}


SongDb::SongDb(string host, int port, int timeout)
{
	m_mpd = mpd_newConnection(host.c_str(), port, timeout);
	int rc = sqlite3_open("songdb", &m_db);
	if(rc) {
		sqlite3_close(m_db);
		//throw runtime_error("Unable to open song db");
		cout << "unable to open db" << endl;
	}
}

SongDb::~SongDb() {
	mpd_closeConnection(m_mpd);
	sqlite3_close(m_db);
}

void SongDb::update()
{
cout << "updating song db" << endl;
	mpd_Status * mpdStatus;

	mpd_sendStatusCommand(m_mpd);
	mpdStatus = mpd_getStatus(m_mpd);
	mpd_finishCommand(m_mpd);
	while(mpdStatus->updatingDb == 1) { 
		sleep(7);
		mpd_sendStatusCommand(m_mpd);
		mpdStatus = mpd_getStatus(m_mpd);
		mpd_finishCommand(m_mpd);
	}
	mpd_sendListallInfoCommand(m_mpd, "");
	mpd_InfoEntity* mpdItem = mpd_getNextInfoEntity(m_mpd);
	string stmt;
	int rc = 0;
	rc = sqlite3_exec(m_db, "delete from songs", callback, 0, &m_errMsg);
	if(rc != SQLITE_OK) {
		char msg[512];
		sprintf(msg, "sql error: %s", m_errMsg);
		cout << msg << endl;
		cout << "unable to dele" << msg << endl;
		throw runtime_error(msg);
	}
	rc = sqlite3_exec(m_db, "begin transaction", callback, 0, &m_errMsg);
	if(rc != SQLITE_OK) {
		char msg[512];
		sprintf(msg, "sql error: %s", m_errMsg);
		cout << msg << endl;
	}
	int numsongs = 0;
	while(mpdItem != NULL) {
		if(mpdItem->type == 1) {  //song
			stmt = "insert into songs values(";
			stmt.append("\"");
			stmt.append(mpdItem->info.song->file); 
			stmt.append("\",\"");
			if(mpdItem->info.song->album != NULL && strcmp(mpdItem->info.song->album, "")!=0)
				stmt.append(cleanDoubleQuotes(mpdItem->info.song->album)); 
			else
				stmt.append("--Unknown--");
			stmt.append("\",\"");
			if(mpdItem->info.song->title != NULL && strcmp(mpdItem->info.song->title, "")!=0)
				stmt.append(cleanDoubleQuotes(mpdItem->info.song->title));
			else {
				string name(mpdItem->info.song->file);
				name = cleanDoubleQuotes(name.substr(name.rfind("/")+1));
				stmt += name;
			}
			stmt.append("\",\"");
			if(mpdItem->info.song->artist != NULL && strcmp(mpdItem->info.song->artist, "")!=0)
				stmt.append(cleanDoubleQuotes(mpdItem->info.song->artist));
			else
				stmt.append("--Unknown--");
			stmt.append("\",\"");
			if(mpdItem->info.song->genre != NULL && strcmp(mpdItem->info.song->genre, "")!=0)
				stmt.append(cleanDoubleQuotes(mpdItem->info.song->genre)); 
			else
				stmt.append("--Unknown--");
			stmt.append("\")");
			rc = sqlite3_exec(m_db, stmt.c_str(), callback, 0, &m_errMsg);
			if(rc != SQLITE_OK) {
				char msg[512];
				sprintf(msg, "sql error: %s", m_errMsg);
				cout << msg << endl;
				throw runtime_error(msg);
			}
			++numsongs;
//cout << mpdItem->info.song->file << endl;
		}
		stmt.clear();	
		mpd_freeInfoEntity(mpdItem);
		mpdItem = mpd_getNextInfoEntity(m_mpd);
	}
	cout << "committing " << numsongs << " songs" << endl;
	rc = sqlite3_exec(m_db, "commit transaction", callback, 0, &m_errMsg);
	if(rc != SQLITE_OK) {
		char msg[512];
		sprintf(msg, "sql error: %s", m_errMsg);
		cout << msg << endl;
	}
cout << "updated song db" << endl;
}

std::string SongDb::cleanDoubleQuotes(std::string inStr)
{
	size_t pos = inStr.find("\"");
	while(pos != string::npos) {
		inStr.replace(pos, 1, "'");
		pos = inStr.find("\"");
	}

	return inStr;
}

SongDb::songsAndPaths_t SongDb::getSongsInAlbum(std::string album ,std::string artist)
{
	resultSet.clear();
	vector<std::string> songs;
	string stmt;
	stmt = "select path, title from songs where album = \"";
	stmt += album;
	stmt += "\"";
	if(artist.length() > 0) {
		stmt += " and artist = \"";
		stmt += artist;
		stmt += "\"";
	}
	cout << "stmt " << stmt << endl;
	int rc = sqlite3_exec(m_db, stmt.c_str(), callback, 0, &m_errMsg);
	if(rc != SQLITE_OK) {
		char msg[512];
		sprintf(msg, "sql error: %s", m_errMsg);
		cout << msg << endl;
		throw runtime_error(msg);
	}
	return resultSet;
}

SongDb::songsAndPaths_t SongDb::getSongsInFolder(std::string path)
{
	resultSet.clear();
	vector<std::string> songs;
	string stmt = "select path, title from songs where path like '";
	stmt += path;
	stmt += "%' order by title";
	int rc = sqlite3_exec(m_db, stmt.c_str(), callback, 0, &m_errMsg);
	if(rc != SQLITE_OK) {
		char msg[512];
		sprintf(msg, "sql error: %s", m_errMsg);
		cout << msg << endl;
		throw runtime_error(msg);
	}

	return resultSet;
}

SongDb::songsAndPaths_t SongDb::getSongsForArtist(std::string artist)
{
	resultSet.clear();
	vector<std::string> songs;
	string stmt = "select path, title from songs where artist = \"";
	stmt += artist;
	stmt += "\" order by title";
	int rc = sqlite3_exec(m_db, stmt.c_str(), callback, 0, &m_errMsg);
	if(rc != SQLITE_OK) {
		char msg[512];
		sprintf(msg, "sql error: %s", m_errMsg);
		cout << msg << endl;
		throw runtime_error(msg);
	}

	return resultSet;
}

SongDb::songsAndPaths_t SongDb::getSongs()
{
	resultSet.clear();
	vector<std::string> songs;
	string stmt = "select path, title from songs order by title";
	int rc = sqlite3_exec(m_db, stmt.c_str(), callback, 0, &m_errMsg);
		cout << "stmt " << stmt << endl;
	if(rc != SQLITE_OK) {
		char msg[512];
		sprintf(msg, "sql error: %s", m_errMsg);
		cout << msg << endl;
		throw runtime_error(msg);
	}

	return resultSet;
}

SongDb::albums_t SongDb::getAlbums(string filter)
{
	singleResult.clear();
	string stmt = "select distinct(album) from songs order by album";
	if(!filter.empty()) 
		stmt = "select distinct(album) from songs where album like '" + filter + "' order by album";
	int rc = sqlite3_exec(m_db, stmt.c_str(), callback2, 0, &m_errMsg);
	if(rc != SQLITE_OK) {
		char msg[512];
		sprintf(msg, "sql error: %s", m_errMsg);
		cout << msg << endl;
		throw runtime_error(msg);
	}

	return singleResult;
}

SongDb::albums_t SongDb::getArtists(string filter)
{
	singleResult.clear();
	string stmt = "select distinct(artist) from songs order by artist";
	if(!filter.empty()) 
		stmt = "select distinct(artist) from songs where artist like '" + filter + "' order by artist";
	int rc = sqlite3_exec(m_db, stmt.c_str(), callback2, 0, &m_errMsg);
	if(rc != SQLITE_OK) {
		char msg[512];
		sprintf(msg, "sql error: %s", m_errMsg);
		cout << msg << endl;
		throw runtime_error(msg);
	}

	return singleResult;
}

SongDb::albums_t SongDb::getGenres(string filter)
{
	singleResult.clear();
	string stmt = "select distinct(genre) from songs order by genre";
	if(!filter.empty()) 
		stmt = "select distinct(genre) from songs where genre like '" + filter + "' order by genre";
	int rc = sqlite3_exec(m_db, stmt.c_str(), callback2, 0, &m_errMsg);
	if(rc != SQLITE_OK) {
		char msg[512];
		sprintf(msg, "sql error: %s", m_errMsg);
		cout << msg << endl;
		throw runtime_error(msg);
	}

	return singleResult;
}

SongDb::albums_t SongDb::getAlbumsInGenre(string genre)
{
	singleResult.clear();
	string stmt = "select distinct(album) from songs where genre=\"";
	stmt += genre;
	stmt += "\" order by album";
	int rc = sqlite3_exec(m_db, stmt.c_str(), callback2, 0, &m_errMsg);
	if(rc != SQLITE_OK) {
		char msg[512];
		sprintf(msg, "sql error: %s", m_errMsg);
		cout << msg << endl;
		throw runtime_error(msg);
	}

	return singleResult;
}

SongDb::songsAndPaths_t SongDb::getSongsInGenre(std::string genre, std::string album)
{
	resultSet.clear();
	vector<std::string> songs;
	string stmt;
	stmt = "select path, title from songs where genre = \"";
	stmt += genre;
	stmt += "\"";
	if(!album.empty()) {
		stmt += " and album = \"";
		stmt += album;
		stmt += "\"";
	}
	cout << "stmt " << stmt << endl;
	int rc = sqlite3_exec(m_db, stmt.c_str(), callback, 0, &m_errMsg);
	if(rc != SQLITE_OK) {
		char msg[512];
		sprintf(msg, "sql error: %s", m_errMsg);
		cout << msg << endl;
		throw runtime_error(msg);
	}
	return resultSet;
}
