#mir_tag_start end_com
#mir_tag_data <<SET_ID>>
{
  int x;
  int y;
}
#mir_tag_end

#mir_tag_start end_com_cleanup
#mir_tag_data <<SET_ID>>
{
  free(x->str);
}
#mir_tag_end
