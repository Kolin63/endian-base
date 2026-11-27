#mir_tag_start end_com
#mir_tag_data <<SET_ID>>
{
  int x;
  int y;
}
#mir_tag_end

// void end_com_%n_%d_new(struct end_com_%n_%d* x)
#mir_tag_start end_com_new
#mir_tag_data <<SET_ID>>
{
  x->foo = 10;
}
#mir_tag_end

// void end_com_%n_%d_cleanup(struct end_com_%n_%d* x)
#mir_tag_start end_com_cleanup
#mir_tag_data <<SET_ID>>
{
  free(x->str);
}
#mir_tag_end
