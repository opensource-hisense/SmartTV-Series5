/* This source file was automatically created by the */
/* tool 'MTK RPC Description tool', 'Version 1.10' on 'Wed Nov 15 16:45:33 2017'. */
/* Do NOT modify this source file. */



/* Start of source pre-amble file 'src_header_file.h'. */

#include "mtk_bt_service_spp_ipcrpc_struct.h"

/* End of source pre-amble file 'src_header_file.h'. */





static const RPC_DESC_T* at_rpc_desc_list [] =
{
};

EXPORT_SYMBOL const RPC_DESC_T* __rpc_get_spp_desc__ (UINT32  ui4_idx)
{
  return ((ui4_idx < 0) ? at_rpc_desc_list [ui4_idx] : NULL);
}


