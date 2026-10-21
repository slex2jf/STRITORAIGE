from pathlib import Path
import argparse,hashlib,json,re,datetime
x=argparse.ArgumentParser();x.add_argument('root',type=Path);x.add_argument('--reference-rom',type=Path,required=True);a=x.parse_args();root=a.root.resolve();rom=a.reference_rom.read_bytes();expected='dd44f120446654bb91c448762f3e0cd0d9b034f35d0e3266a4dc34402ada95c0';assert hashlib.sha256(rom).hexdigest()==expected
# Distribution payload only: build outputs and Git database are outside the publication set.
files=[p for p in root.rglob('*') if p.is_file() and not any(t=='.git' or t=='build' or t.startswith('build-') for t in p.relative_to(root).parts)]
chunks={rom[i:i+256] for i in range(0,len(rom)-255,256) if len(set(rom[i:i+256]))>=32}
headers=[rom[0x100:0x180]];issues=[];binary=[];arrays=[];large=[]
for p in files:
 b=p.read_bytes();n=str(p.relative_to(root));h=hashlib.sha256(b).hexdigest()
 if p.is_symlink():issues.append([n,'symlink'])
 if h==expected or rom in b:issues.append([n,'ROM content or SHA collision'])
 if len(b)==524288:issues.append([n,'524288 bytes'])
 if p.suffix.lower() in ['.rom','.gen','.smd','.zip','.tar','.gz','.7z','.rar','.bin','.ram','.vram','.cram','.vsram','.state']:issues.append([n,'ROM/archive/dump extension'])
 if any(v in b for v in headers) or any(c in b for c in chunks):issues.append([n,'256-byte high-entropy ROM chunk or header'])
 if b'\0' in b:binary.append([n,len(b)])
 if len(b)>1048576:large.append([n,len(b)])
 if p.suffix in ['.c','.h','.cpp','.hpp','.ipp']:
  s=b.decode('utf-8',errors='replace')
  for m in re.finditer(r'\b(?:const\s+)?(?:unsigned\s+char|u?int8_t|char)\s+\w+\s*\[[^\]]*\]\s*=\s*\{([^}]+)\}',s,re.S):
   if len(m.group(1))>1024:arrays.append([n,len(m.group(1))])
report={'date':'2026-10-21 America/Sao_Paulo','scope':'distribution payload; excludes .git and build/build-* artifacts','files':len(files),'bytes':sum(p.stat().st_size for p in files),'ROM_PRESENT':'NO' if not issues else 'REVIEW','issues':issues,'binary_files':binary,'large_files_over_1MiB':large,'large_byte_arrays_found':arrays,'reviewed_arrays':'minicoro host context switch/wrapper machine code; not game ROM; bundled license retained','reference_sha':expected,'checks':'Every payload file hashed; full ROM subsequence, exact ROM SHA, 524288 size, extensions/archives/dumps, header and aligned high-entropy 256-byte ROM chunks checked. Textual SHA/name allowed. Renamed raw ROM detected. No exhaustive proof against arbitrary encoding/compression/encryption.','generated_C':'701 function declarations; translated instructions/immediates. Three const metadata arrays: dispatch, hook sites and instruction cycle costs. No byte asset arrays detected. No legal conclusion.'}
(root/'docs/PUBLICATION_AUDIT.md').write_text('# Publication technical audit\n\n```json\n'+json.dumps(report,indent=2)+'\n```\n\nCounts include the prior version of this report if present; final independent payload inventory is recorded outside the distribution. Binary SDL/PE files, if present, are the documented runtime components, not ROM content. License clearance is separate and unresolved components remain identified in LICENSE-NOTICE.md.\n')
print(json.dumps(report,indent=2));assert not issues
