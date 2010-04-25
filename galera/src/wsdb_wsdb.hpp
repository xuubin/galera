//
// Copyright (C) 2010 Codership Oy <info@codership.com>
//

#ifndef GALERA_WSDB_WSDB_HPP
#define GALERA_WSDB_WSDB_HPP

#include "wsdb.hpp"
#include "gu_mutex.hpp"
#include <boost/unordered_map.hpp>

namespace galera
{
    class WsdbWsdb : public Wsdb
    {
        typedef boost::unordered_map<wsrep_trx_id_t, TrxHandlePtr> TrxMap;
        typedef boost::unordered_map<wsrep_conn_id_t, TrxHandlePtr> ConnQueryMap;
    public:
        
        // Get trx handle from wsdb
        TrxHandlePtr get_trx(wsrep_trx_id_t id, bool create = false);
        TrxHandlePtr get_conn_query(wsrep_conn_id_t id, bool create = false);
        // Discard trx handle
        void discard_trx(wsrep_trx_id_t id);
        void discard_conn_query(wsrep_conn_id_t id);
        
        void append_query(TrxHandlePtr&, const void* query, size_t query_len);
        void append_row_key(TrxHandlePtr&,
                            const void* dbtable, 
                            size_t dbtable_len,
                            const void* key, 
                            size_t key_len,
                            int action);
        
        void create_write_set(TrxHandlePtr&, 
                              const void* rbr_data = 0,
                              size_t rbr_data_len = 0);
        
        WsdbWsdb() : trx_map_(), conn_query_map_(), mutex_() { }
        ~WsdbWsdb() { }
    private:
        // Create new trx handle
        TrxHandlePtr create_trx(wsrep_trx_id_t trx_id);
        TrxHandlePtr create_conn_query(wsrep_conn_id_t conn_id);
    private:
        TrxMap       trx_map_;
        ConnQueryMap conn_query_map_;
        gu::Mutex        mutex_;
    };
}

#endif // GALERA_WSDB_WSDB_HPP