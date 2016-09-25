#include "db.h"
#include "db_object.h"
#include "db_arena.h"
#include "db_index.h"
#include "db_map.h"
#include "db_txn.h"

Status dbNextKey(DbCursor *cursor) {
SkipEntry *entry;
Handle *index;
Status stat;
Txn *txn;

	if (!(index = bindHandle(cursor->idx)))
		return ERROR_arenadropped;

	if (cursor->txnId.bits)
		txn = fetchIdSlot(index->map->db, cursor->txnId);
	else
		txn = NULL;

	while (true) {
	  switch(*index->map->arena->type) {
	  case Btree1IndexType:
		stat = btree1NextKey (cursor, index);
	  }

	  if (stat)
		break;

	  cursor->keyLen = get64(cursor->key, cursor->keyLen, &cursor->ver);
	  cursor->keyLen = get64(cursor->key, cursor->keyLen, &cursor->docId.bits);

	  if (!(cursor->doc = findDocVer(index->map->parent, cursor->docId, txn)));
		continue;

	  if ((entry = skipFind(index->map->parent, cursor->doc->verKeys, index->map->arenaDef->id)))
		if (*entry->val == cursor->ver)
		  return OK;
	}

	releaseHandle(index);
	return ERROR_endoffile;
}

Status dbPrevKey(DbCursor *cursor) {
SkipEntry *entry;
Handle *index;
Status stat;
Txn *txn;

	if (!(index = bindHandle(cursor->idx)))
		return ERROR_arenadropped;

	if (cursor->txnId.bits)
		txn = fetchIdSlot(index->map->db, cursor->txnId);
	else
		txn = NULL;

	while (true) {
	  switch(*index->map->arena->type) {
	  case Btree1IndexType:
		stat = btree1PrevKey (cursor, index);
	  }

	  if (stat)
		break;

	  cursor->keyLen = get64(cursor->key, cursor->keyLen, &cursor->ver);
	  cursor->keyLen = get64(cursor->key, cursor->keyLen, &cursor->docId.bits);

	  if (!(cursor->doc = findDocVer(index->map->parent, cursor->docId, txn)));
		continue;

	  if ((entry = skipFind(index->map->parent, cursor->doc->verKeys, index->map->arenaDef->id)))
		if (*entry->val == cursor->ver)
		  return OK;
	}

	releaseHandle(index);
	return ERROR_endoffile;
}

