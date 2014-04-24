#include <iostream>
#include <vector>
#include <string>
#include <SFML/Audio.hpp>
#include "string_utils.h"
#include <windows.h>
#include <fstream>

std::string lastError;
std::vector< std::string > tracks;
sf::Music* music( 0 );

void addTrack( const std::string& path )
{
    std::ifstream f( path.c_str() );
    if( f.good() )
    {
        f.close();
        tracks.push_back( path );
    }
    else
    {
        f.close();
        lastError = "File does not exists: '" + path + "'";
    }
}

void loadPlaylist()
{
    std::ifstream f( "playlist.dat" );
    if( f.good() )
    {
        std::string path;
        while( std::getline( f, path ) )
            addTrack( path );
        f.close();
    }
    else
        f.close();
}

void savePlaylist()
{
    std::ofstream f( "playlist.dat" );
    if( f.good() )
    {
        for( unsigned int i = 0; i < tracks.size(); i++ )
            f << tracks[ i ] << std::endl;
        f.flush();
        f.close();
    }
    else
        f.close();
}

void stopPlaying()
{
    if( music )
    {
        music->stop();
        delete music;
    }
    music = 0;
}

void startPlaying( unsigned int track )
{
    stopPlaying();
    if( track >= tracks.size() )
    {
        lastError = "Track not found!";
        return;
    }
    music = new sf::Music();
    if( !music->openFromFile( tracks[ track ] ) )
    {
        stopPlaying();
        lastError = "Cannot play track: '" + tracks[ track ] + "'";
        return;
    }
    music->setLoop( true );
    music->play();
}

void clearScreen()
{
    if( system( "CLS" ) )
        system( "clear" );
    std::cout << ">>> ConsoleAudioPlayer by PsichiX <<<" << std::endl << std::endl;
}

void displayPlaylist()
{
    std::cout << "=== Playlist ===" << std::endl;
    for( unsigned int i = 0; i < tracks.size(); i++ )
        std::cout << i << ". " << tracks[ i ].c_str() << std::endl;
}

void displayHelp()
{
    std::cout << "=== Commands ===" << std::endl;
    std::cout << "help, ? - show this help" << std::endl;
    std::cout << "exit - exit application" << std::endl;
    std::cout << "add <path> - add track from <path>" << std::endl;
    std::cout << "remove <index> - remove track at <index>" << std::endl;
    std::cout << "play <index> - play track at <index>" << std::endl;
    std::cout << "stop - stop playing track" << std::endl;
}

void processPlaylist( const std::string& action )
{
    std::vector< std::string > cmds = string_split( action, ' ' );
    if( cmds.size() <= 0 )
        return;
    if( cmds[ 0 ] == "add" && cmds.size() >= 2 )
    {
        std::string path = string_join( " ", ++cmds.begin(), cmds.end() );
        addTrack( path );
    }
    else if( cmds[ 0 ] == "remove" && cmds.size() >= 2 )
    {
        std::stringstream ss( cmds[ 1 ] );
        unsigned int vb = MAXDWORD;
        ss >> vb;
        if( vb < tracks.size() )
            tracks.erase( tracks.begin() + vb );
        else
            lastError = "Track not found!";
    }
    else if( cmds[ 0 ] == "play" && cmds.size() >= 2 )
    {
        std::stringstream ss( cmds[ 1 ] );
        unsigned int vb = MAXDWORD;
        ss >> vb;
        startPlaying( vb );
    }
    else if( cmds[ 0 ] == "stop" && cmds.size() >= 1 )
    {
        stopPlaying();
    }
}

int main()
{
    atexit( stopPlaying );
    int mode = -1;
    loadPlaylist();
    while( true )
    {
        clearScreen();
        if( mode == -1 )
        {
            displayHelp();
            std::cout << std::endl;
            mode = 0;
        }
        if( mode == 0 )
            displayPlaylist();
        if( !lastError.empty() )
        {
            std::cout << std::endl << "ERROR: " << lastError << std::endl;
            lastError.clear();
        }
        std::cout << std::endl << "Command: ";
        std::string action;
        std::getline( std::cin, action );
        if( action == "exit" )
            break;
        else if( action == "help" || action == "?" )
            mode = -1;
        else if( mode == 0 )
            processPlaylist( action );
    }
    stopPlaying();
    savePlaylist();
    return 0;
}
