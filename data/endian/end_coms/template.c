#mir_tag_start end_com
#mir_tag_data << dont forget to set id in tag data>>
{
  int x;
  int y;
}
#mir_tag_end

#mir_tag_start end_com_cleanup
#mir_tag_data << dont forget to set id in tag data>>
{
  free(x->str);
}
#mir_tag_end
