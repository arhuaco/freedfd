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
  foreach my $key (sort {
                           my $max = length($a);
                           if (length($b) > $max)
                           {
                             $max = length($b);
                           }
                           $max - length($a) <=> $max - length ($b)
                         } (keys %S))
  {
    if ($orig =~ m/$key/)
    {
      $orig =~ s/$key/$S{$key}/;
    }
  }
  print $orig;
}

# To generate the intermediate file:
# my $f = "";
#
#
# while(<>)
# {
# # MENUITEM "&Acercar\t+        ", CM_VERZOOMMAS
# if (/\s*(CM_\w+),\s*(\".*?\")/)
# {
#   $f .=  "  #define HELP_$1 $2\n";
#   print "$1, HELP_$1\n"
# }
# else
# {
#   die "error";
# }
# }
# print "\n\n" . $f;
