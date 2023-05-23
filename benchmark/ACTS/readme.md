### kiwaku

Compile with kiwaku :

In the project build/
```
cmake ../cmake -DCPM_kiwaku_SOURCE=/home/higepi/stage/kiwaku -G Ninja -DCMAKE_CXX_COMPILER=clang++-15
```

Make launch
``` 
ninja -v kwk.out
```

## Benchmark

Table_find = Cellules dans une table kiwaku + std::find

Table_lower_bound = Cellules dans une table + std::lower_bound

view_find = view sur cellules présentes + std::find

view_lower_bound = view + std::lower_bound

find_conn_view_lower_bound = seulement find_connection avec view_lower_bound

view_lb_b_to_index = view_lower_bound + recherche des connections entre début et position actuelle

view_lb_index_index = view_lb_b_to_index + recherche entre position et max(position - N, 0) 

v_lb_connections_previous = find connection version acts en remontant -1 en -1