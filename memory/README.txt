memory:
  
ZONE(1M)
    |
    |
    ->CHUNK(2kb)->CHUNK(2kb)->CHUNK(2kb)->CHUNK(2kb)->
ZONE(4M)
    |
    |
    ->CHUNK(4kb)->CHUNK(4kb)->CHUNK(4kb)->CHUNK(4kb)->
ZONE(8M)
    |
    |
    ->CHUNK(16kb)->CHUNK(16kb)->CHUNK(16kb)->CHUNK(16kb)->
ZONE(32M)
    |
    |
    ->CHUNK(64kb)->CHUNK(64kb)->CHUNK(64kb)->CHUNK(64kb)->


ZONE(1M)->ZONE(4M)->ZONE(8M)->ZONE(32M)
