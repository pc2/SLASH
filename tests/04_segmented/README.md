# Segmented design test

This test shows usage of the segmented design configuration. For the segmented design to work, partition 1 of the OSPI should be flashed with the image in <prj_root>/deploy. To flash the OSPI memory run:

```bash
sudo ami_tool cfgmem_program -d bb:dd.f -i <prj_root>/deploy/desgin.pdi -p 1 -t primary -q
```

After this, a reboot is required.

## Note

This is a one-time operation.