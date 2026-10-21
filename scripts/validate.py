from pathlib import Path
import argparse,csv,hashlib,os,subprocess
P=Path(__file__).resolve().parents[1]
a=argparse.ArgumentParser();a.add_argument('--rom',type=Path,default=P/'rom/Streets of Rage.bin');a.add_argument('--exe',type=Path,default=P/'build/sor1_visual_case4');a.add_argument('--frames',type=int,nargs='+',default=[500,1000,3515,10000]);a.add_argument('--out',type=Path,default=P/'build-validation');v=a.parse_args()
assert hashlib.sha256(v.rom.read_bytes()).hexdigest()=='dd44f120446654bb91c448762f3e0cd0d9b034f35d0e3266a4dc34402ada95c0','ROM SHA mismatch'
for n in v.frames:
 rows=[]
 for backend in ['interp','hybrid']:
  d=v.out.resolve()/str(n)/backend;d.mkdir(parents=True,exist_ok=False)
  env=os.environ.copy()
  for key in ['GENESIS_FORCE_INTERP','GENESIS_COSIM_WAITVBL_PC','CONV_COMMON_TIMING','CONV_BOUNDARY_TIMING','CONV_TRACE_FRAME','CYCLE_EVENTS']:env.pop(key,None)
  env['SDL_GAMECONTROLLER_IGNORE_DEVICES_EXCEPT']='0x0000/0x0000'
  env.update(SOR_STATE_HASHES='1',SOR_HEAVY_DIAG='0',SDL_AUDIODRIVER='dummy',SOR_PROFILE_DIR=str(d))
  if backend=='interp':env['GENESIS_FORCE_INTERP']='1'
  with (d/'stdout.log').open('w') as o,(d/'stderr.log').open('w') as e:subprocess.run([str(v.exe.resolve()),str(v.rom.resolve()),'--turbo','--max-frames',str(n),'--input-script',str(P/'tests/smoke.input')],cwd=d,env=env,stdout=o,stderr=e,check=True,timeout=300)
  rows.append(list(csv.DictReader((d/'state-frames.csv').open())))
 assert all(len(r)==n for r in rows),(n,[len(r) for r in rows])
 for i,(x,y) in enumerate(zip(*rows)):
  for k in ['frame','cpu','ram','vram','cram','vsram','regs','vdp','master','cycles']:assert x[k]==y[k],(n,i,k,x[k],y[k])
 print('strict',n,'PASS',flush=True)
