#!/bin/sh

compiler='g++'
sources=''
headers='src/network/http/*.hpp src/network/tcp/*.hpp src/general/*.hpp src/events/*.hpp'
flags='-std=c++14'
base_build="$compiler $sources $headers $flags"

server_sources='src/example/server.cpp'
server_output='-o bin/httpServer'

client_sources='src/example/client.cpp'
client_output='-o bin/httpClient'

echo 'Building Server Program'
$base_build $server_sources $server_output
echo 'Finished Server Program'
echo
echo 'Building Client Program'
$base_build $client_sources $client_output
echo 'Finished Client Program'