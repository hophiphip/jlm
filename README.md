# jlm
Just a Linux Module

# Requirements
`build-essential` `linux-headers`

# Installation
### Compile
```bash
  make 
```
### Install module
```bash
  make install
```
### Create a device file
```bash
  make device
```
### Remove module
```bash
  make remove
```

# Usage
### Try as a normal user
```bash
  $ cat /dev/jlm
```
Will output:
```text
jlm device
jlm device
jlm device
...
```

### As root
```bash
  $ sudo cat /dev/jlm
```
Or
```bash
  # cat /dev/jlm
```
Will output nothing
