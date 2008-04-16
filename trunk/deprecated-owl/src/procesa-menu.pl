#!/usr/bin/perl -w

use strict;

open MSG, '<messages.h';

my %S;

my $lang = shift || '__SPANISH__';
my $is_in = 0;

while(<MSG>)
{
  if (/$lang/)
  {
    $is_in = 1 - $is_in
  }
  for my $prefix ('TXT_CM_', 'TXT_MENU_', 'HELP_CM_')
  {
    if ($is_in && /define\s+($prefix\w+)\s+(\".*?\")/)
    {
      $S{$1} = $2;
    }
  }
}

close MSG;

open RC, '<dfd.rc.source';

while (<RC>)
{
  my $orig = $_;
  foreach my $key (keys %S)
  {
    if ($orig =~ m/$key/)
    {
      $orig =~ s/$key/$S{$key}/;
    }
  }
  print $orig;
}
