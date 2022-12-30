# iSH-LD_PRELOAD-Hack

Stub some syscalls to run programs which expect syscalls that don't exist in iSH.
The syscalls return -1 indicating failure, so the program will hopefully recognise
that the syscall is bogus. The stubbed syscalls are mostly esoteric enough that they
shouldn't break the program's execution.

## Usage

Compile the hack with

```gcc -O2 -shared -o /usr/local/lib/<name>.so <name>.c```

Use it with

```LD_PRELOAD=/usr/local/lib/<name>.so <program>```

You'll probably want to alias the program to do that automatically, for example by
adding the following to a shell startup script like `.profile`:

```alias qemu='LD_PRELOAD=/usr/local/lib/qemu-hack.so qemu-system-i386'```

## Why?

Because iSH doesn't include these stubbed syscalls yet. If you're tired of using this
hack, contribute to [iSH](https://github.com/ish-app/ish) and stub (or implement, if
you're feeling ambitious) the syscalls there.
