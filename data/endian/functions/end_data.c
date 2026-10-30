#mir_tag_start PREPROC
#include "base/end_data.h"
#mir_tag_end

#mir_tag_start function_init
#mir_tag_data init_data
end_init_data();
#mir_tag_end

#mir_tag_start function_load
#mir_tag_data load_data
end_load_data();
#mir_tag_end

#mir_tag_start function_save
#mir_tag_data save_data
end_save_data();
#mir_tag_end

#mir_tag_start function_cleanup
#mir_tag_data cleanup_data
end_cleanup_data();
#mir_tag_end
